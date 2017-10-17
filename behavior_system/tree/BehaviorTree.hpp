//
// Created by draghan on 2017-10-15.
//

#pragma once

#include <algorithm>
#include "../IBehavior.hpp"

class BehaviorTree
{
public:
    using id_t = uint32_t;

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
        operation_correct = true;
        std::initializer_list<int>{(go_to_node_relatively(args), 0)...};
        return operation_correct;
    }

    bool set_at_relatively()
    {
        return true;
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
        return true;
    }

    bool set_at_id(id_t wanted_id)
    {
        auto found = std::find_if(nodes.begin(), nodes.end(), [&](auto &node) {
            return (*node)->get_id() == wanted_id;
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

    void add_child(IBehavior::ptr &&child)
    {
        nodes.emplace_back(child);
        auto added_node = get_last_node();
        added_node->set_id(last_id);
        ++last_id;
        active->add_child(added_node);
    }

    void print(std::ostream &stream)
    {
        bool root_is_lonely{root->get_number_of_children() == 0};
        root->PrintPretty("", root_is_lonely, stream);
    }

private:
    std::vector<IBehavior::ptr> nodes;
    IBehavior::ptr root;
    IBehavior::ptr active;
    id_t last_id;
    bool operation_correct;

    IBehavior::ptr get_last_node()
    {
        return *(nodes.end() - 1);
    }

    void go_to_node_relatively(size_t x)
    {
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



