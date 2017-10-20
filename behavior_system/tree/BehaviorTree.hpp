//
// Created by draghan on 2017-10-15.
//

#pragma once

#include <algorithm>
#include <limits>
#include "../IBehavior.hpp"
#include "../composite/BehaviorSelector.hpp"
#include "../composite/BehaviorSequence.hpp"
#include "../primitive/BehaviorAction.hpp"
#include "../primitive/BehaviorCondition.hpp"

class BehaviorTree
{
public:
    using id_t = uint32_t;
    const static id_t undefined_id;

    BehaviorTree(IBehavior::ptr root)
            : nodes{1, root},
              root{nodes[0]},
              active{root},
              last_id{0},
              operation_correct{true}
    {
        root->set_id(last_id);
        ++last_id;
    }

    BehaviorTree()
            : nodes{},
              root{nullptr},
              active{nullptr},
              last_id{0},
              operation_correct{true}
    {
    }

    BehaviorTree(const BehaviorTree &) = delete;

    BehaviorTree(BehaviorTree &&bt) noexcept
            : nodes{std::move(bt.nodes)},
              root{bt.root},
              active{bt.active},
              last_id{bt.last_id},
              operation_correct{bt.operation_correct}
    {
        bt.root = nullptr;
        bt.active = nullptr;
        bt.last_id = 0;
    }

    BehaviorTree &operator=(const BehaviorTree &bt) = delete;

    BehaviorTree &operator=(BehaviorTree &&bt) noexcept
    {
        delete_nodes();

        nodes = std::move(bt.nodes);
        root = bt.root;
        active = bt.active;
        last_id = bt.last_id;
        operation_correct = bt.operation_correct;

        bt.nodes.clear();
        bt.root = nullptr;
        bt.active = nullptr;
        return *this;
    }

    BehaviorTree &operator+=(
            BehaviorTree &&bt) noexcept // todo check if it is required to relocate vectors of children in moved nodes
    {
        std::move(std::begin(bt.nodes), std::end(bt.nodes), std::back_inserter(nodes));
        bt.nodes.clear();
        bt.root = nullptr;
        bt.active = nullptr;

        for (size_t id = last_id; id < nodes.size(); ++id)
        {
            nodes[id]->set_id(id);
        }
        last_id = nodes.size();

        return *this;
    }

    ~BehaviorTree()
    {
        delete_nodes();
    }

    template<typename... Args>
    bool set_at_relatively(const Args &... args)
    {
        if (active == nullptr)
        {
            operation_correct = false;
            return false;
        }

        operation_correct = true;
        std::initializer_list<int>{(go_to_node_relatively(args), 0)...};
        return operation_correct;
    }

    bool set_at_relatively()
    {
        if (active == nullptr)
        {
            return false;
        } else
        {
            return true;
        }
    }

    template<typename... Args>
    bool set_at_absolutely(const Args &... args)
    {
        back_to_root();
        return set_at_relatively(args...);
    }

    bool set_at_absolutely()
    {
        back_to_root();
        if (active == nullptr)
        {
            return false;
        } else
        {
            return true;
        }
    }

    bool set_at_id(id_t wanted_id)
    {
        auto found = std::find_if(nodes.begin(), nodes.end(), [&](auto &node) {
            return node->get_id() == wanted_id;
        });

        if (found == nodes.end())
        {
            return false;
        } else
        {
            active = *found;
            return true;
        }
    }

    IBehavior::ptr get()
    {
        return active;
    }

    id_t get_id() const
    {
        if (active == nullptr)
        {
            return undefined_id;
        }
        return active->get_id();
    }

    id_t get_node_count() const
    {
        return nodes.size();
    }

    bool add_child(IBehavior::ptr &&child)
    {
        try
        {
            nodes.emplace_back(child);
        }
        catch (...)
        {
            return false;
        }
        if (nodes.size() == 1)
        {
            root = nodes[0];
            active = root;
        }
        auto added_node = get_last_node();
        if (added_node == nullptr)
        {
            return false;
        }
        added_node->set_id(last_id);
        ++last_id;
        return active->add_child(added_node);
    }

    bool add_sequence()
    {
        return add_child(new BehaviorSequence(id_any, active));
    }

    bool add_selector()
    {
        return add_child(new BehaviorSelector(id_any, active));
    }

    bool add_action(BehaviorAction::action_t &&action)
    {
        return add_child(new BehaviorAction(id_any, std::move(action), active));
    }

    bool add_condition(BehaviorCondition::predicate_t &&predicate)
    {
        return add_child(new BehaviorCondition(id_any, std::move(predicate), active));
    }


    void print(std::ostream &stream)
    {
        bool root_is_lonely{root->get_number_of_children() == 0};
        root->PrintPretty("", root_is_lonely, stream);
    }

    BehaviorState evaluate()
    {
        if (active == nullptr)
        {
            return BehaviorState::undefined;
        } else
        {
            return active->evaluate();
        }
    }

private:
    static const id_t id_any;
    std::vector<IBehavior::ptr> nodes;
    IBehavior::ptr root;
    IBehavior::ptr active;
    id_t last_id;
    bool operation_correct;

    IBehavior::ptr get_last_node()
    {
        if (nodes.size() > 0)
        {
            return *(nodes.end() - 1);
        } else
        {
            return nullptr;
        }
    }

    void go_to_node_relatively(size_t x)
    {
        if (active == nullptr)
        {
            operation_correct = false;
            return;
        }

        auto temp = active->get_child(x);

        if (temp == nullptr)
        {
            operation_correct = false;
        } else
        {
            active = temp;
        }
    }

    void back_to_root()
    {
        active = root;
    }

    void delete_nodes()
    {
        for (auto &node : nodes)
        {
            delete node;
        }
        nodes.clear();
    }
};



