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

    BehaviorEmpty(uint32_t id)
            : IBehavior{id}
    {}

    virtual void print(std::ostream& stream)
    {
        stream << "BehaviorEmpty [" << id << "]\n";
    }

protected:
    BehaviorState internal_evaluate() override
    {
        return BehaviorState::undefined;
    }
};



