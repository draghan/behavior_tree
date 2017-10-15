//
// Created by draghan on 2017-10-15.
//

#pragma once

#include "../IBehavior.hpp"

class BehaviorTree
{
public:
    BehaviorTree();

    ~BehaviorTree();

    bool set_at_relatively();

    bool set_at_absolutely();

    bool set_at_id();

    void add_child(IBehavior::ptr);

    IBehavior::ptr get();

    void print(std::ostream& stream);

private:
    std::vector<IBehavior::ptr> nodes;

    void back_to_root();
};



