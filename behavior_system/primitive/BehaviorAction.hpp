//
// Created by draghan on 2017-10-14.
//

#pragma once


#include <functional>
#include "../IBehavior.hpp"

class BehaviorAction :public IBehavior
{
public:
    bool can_have_children() override
    {
        return false;
    }

    using action_t = std::function<BehaviorState()>;

    BehaviorAction(uint32_t id, action_t action, IBehavior::ptr parent = nullptr) :
            IBehavior(parent, id),
            action{std::move(action)}
    {}

    virtual void print(std::ostream& stream)
    {
        stream << "[ ]\tid = " << id << '\n';
    }

    std::string get_glyph() override
    {
        return "[ ]";
    }

protected:
    BehaviorState internal_evaluate() override
    {
        if(action)
        {
            return action();
        }
        else
        {
            return BehaviorState::undefined;
        }
    }

private:
    action_t action;
};



