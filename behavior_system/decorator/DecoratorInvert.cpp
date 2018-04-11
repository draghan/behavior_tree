//
// Created by draghan on 2017-12-02.
//

#include "DecoratorInvert.hpp"

DecoratorInvert::DecoratorInvert(uint32_t id, IBehavior::ptr parent)
        : IBehavior(parent, id)
{
}

std::string DecoratorInvert::get_glyph()
{
    return "< d > !";
}

bool DecoratorInvert::can_have_children()
{
    return true;
}

BehaviorState DecoratorInvert::internal_evaluate(id_t id)
{
    if(children.empty())
    {
        return BehaviorState::undefined;
    }
    auto result = get_child_for_eval(0)->evaluate();
    if(result == BehaviorState::failure)
    {
        result = BehaviorState::success;
    }
    else if(result == BehaviorState::success)
    {
        result = BehaviorState::failure;
    }
    return result;
}
