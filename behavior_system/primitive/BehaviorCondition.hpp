//
// Created by draghan on 2017-10-14.
//

#pragma once


#include <functional>
#include "../IBehavior.hpp"

class BehaviorCondition :public IBehavior
{
public:
    virtual void print(std::ostream& stream)
    {
        stream << "BehaviorCondition [" << id << "]\n";
    }

    using predicate_t = std::function<bool()>;

    BehaviorCondition(uint32_t id, predicate_t predicate)
            : IBehavior(id), predicate{std::move(predicate)}
    {
    }

    ~BehaviorCondition() override = default;

protected:
    predicate_t predicate;

    BehaviorState internal_evaluate() override
    {
        if(!predicate)
        {
            return BehaviorState::undefined;
        }

        if(predicate())
        {
            return BehaviorState::success;
        }
        else
        {
            return BehaviorState::failure;
        }
    }
};



