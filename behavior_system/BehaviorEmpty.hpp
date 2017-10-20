//
// Created by draghan on 2017-10-14.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include "IBehavior.hpp"

class BehaviorEmpty :public IBehavior
{
public:
    ~BehaviorEmpty() override = default;

    BehaviorEmpty(uint32_t id, IBehavior::ptr parent = nullptr)
            : IBehavior{parent, id}
    {}

    virtual void print(std::ostream& stream)
    {
        stream << ".\tid = " << id << '\n';
    }

    bool can_have_children() override
    {
        return true;
    }

protected:
    BehaviorState internal_evaluate() override
    {
        return BehaviorState::undefined;
    }
};



