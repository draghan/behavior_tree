//
// Created by draghan on 2017-10-14.
//

#pragma once

#include "../IBehavior.hpp"

class BehaviorSequence :public IBehavior
{
public:

    explicit BehaviorSequence(uint32_t id)
            : IBehavior{id}
    {
    }

    ~BehaviorSequence() override = default;

    void print(std::ostream& stream) override
    {
        stream << "[->]\tid = " << id << '\n';
    }

protected:
    BehaviorState internal_evaluate() override
    {
        if(children.empty())
        {
            return BehaviorState::undefined;
        }

        for(auto& child : children)
        {
            if(child->evaluate() != BehaviorState::success)
            {
                return child->get_status();
            }
        }
        return BehaviorState::success;
    }
};



