//
// Created by draghan on 2017-10-14.
//

#pragma once


#include "../IBehavior.hpp"

/*
 * todo Consider if composites (selector, sequence) should do reevaluate from scratch in next clock-tick
 * when some of children returns running state or it should remember which child is running and returns immediately to it.
 * */

class BehaviorSelector :public IBehavior
{
public:
    explicit BehaviorSelector(uint32_t id)
            : IBehavior{id}
    {
    }

    ~BehaviorSelector() override = default;

    void print(std::ostream& stream) override
    {
        stream << "[?]\tid = " << id << '\n';
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
            if(child->evaluate() != BehaviorState::failure)
            {
                return child->get_status();
            }
        }
        return BehaviorState::failure;
    }
};



