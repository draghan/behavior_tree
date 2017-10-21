//
// Created by draghan on 2017-10-14.
//

#pragma once

#include "../IBehavior.hpp"

class BehaviorSequence :public IBehavior
{
public:
    bool can_have_children() override
    {
        return true;
    }

    explicit BehaviorSequence(uint32_t id, IBehavior::ptr parent = nullptr)
            : IBehavior{parent, id}
    {
    }

    ~BehaviorSequence() override = default;

    void print(std::ostream& stream) override
    {
        stream << "[->]\tid = " << id << '\n';
    }

    std::string get_glyph() override
    {
        return "[->]";
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



