//
// Created by draghan on 2017-10-15.
//

#pragma once

#include "../IBehavior.hpp"

class BehaviorTree
{
public:
    BehaviorTree(IBehavior::ptr root)
            : nodes{1, root},
              root{nodes[0]},
              active{root}
    {
    }

    BehaviorTree(const BehaviorTree &) = delete;

    BehaviorTree(BehaviorTree &&bt);

    BehaviorTree &operator=(const BehaviorTree &bt) = delete;

    BehaviorTree &operator=(BehaviorTree &&bt);

    ~BehaviorTree()
    {
        for (auto &node : nodes)
        {
            delete node;
        }
    }

    template<typename... Args>
    bool set_at_relatively(const Args &... args);
    bool set_at_relatively();

    template<typename... Args>
    bool set_at_absolutely(const Args &... args);
    bool set_at_id();

    IBehavior::ptr get();

    void add_child(IBehavior::ptr);

    void print(std::ostream& stream);
private:
    std::vector<IBehavior::ptr> nodes;
    IBehavior::ptr root;
    IBehavior::ptr active;

    void back_to_root();
};



