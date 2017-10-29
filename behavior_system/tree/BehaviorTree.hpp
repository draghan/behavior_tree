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

    explicit BehaviorTree(IBehavior::ptr root);
    BehaviorTree();
    BehaviorTree(const BehaviorTree &) = delete;
    BehaviorTree(BehaviorTree &&bt) noexcept;
    ~BehaviorTree();
    BehaviorTree &operator=(const BehaviorTree &bt) = delete;
    BehaviorTree &operator=(BehaviorTree &&bt) noexcept;
    BehaviorTree &operator+=(BehaviorTree &&bt) noexcept;


    template <typename... Args>
    bool set_at_relatively(const Args &... args);
    bool set_at_relatively();
    template <typename... Args>
    bool set_at_absolutely(const Args &... args);
    bool set_at_absolutely();
    bool set_at_id(id_t wanted_id);

    IBehavior::ptr get();
    id_t get_id() const;
    id_t get_node_count() const;

    [[deprecated("Use factory methods (add_sequence(), add_selector()...) instead.")]]
    bool add_child(IBehavior::ptr &&child);
    bool add_sequence();
    bool add_selector();
    bool add_action(BehaviorAction::action_t &&action);
    bool add_condition(BehaviorCondition::predicate_t &&predicate);

    BehaviorState evaluate();

    void print(std::ostream &stream);
    void clear();
private:
    static const id_t id_any;
    std::vector<IBehavior::ptr> nodes;
    IBehavior::ptr root;
    IBehavior::ptr active;
    id_t last_id;
    bool operation_correct;

    IBehavior::ptr get_last_node();
    void go_to_node_relatively(size_t x);
    void back_to_root();
    void delete_nodes();
};

template <typename... Args>
bool BehaviorTree::set_at_relatively(const Args &... args)
{
    if(active == nullptr)
    {
        operation_correct = false;
        return false;
    }

    operation_correct = true;
    std::initializer_list<int>{(go_to_node_relatively(args), 0)...};
    return operation_correct;
}

template <typename... Args>
bool BehaviorTree::set_at_absolutely(const Args &... args)
{
    back_to_root();
    return set_at_relatively(args...);
}
