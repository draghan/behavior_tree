//
// Created by draghan on 2017-12-03.
//

#include "DecoratorLoop.hpp"

DecoratorLoop::DecoratorLoop(uint32_t repeat, uint32_t id, IBehavior::ptr parent)
        : IBehavior{parent, id},
          max{repeat},
          counter{0},
          reset{true}
{
}

std::string DecoratorLoop::get_glyph()
{
    return "< d > ->N";
}

bool DecoratorLoop::can_have_children()
{
    return true;
}

BehaviorState DecoratorLoop::internal_evaluate(id_t id)
{
    if(children.empty())
    {
        return BehaviorState::undefined;
    }
    if(reset)
    {
        counter = 0;
    }
    for(; counter < max; ++counter)
    {
        auto result = get_child_for_eval(0)->evaluate();
        if(result == BehaviorState::failure)
        {
            reset = true;
            return result;
        }
        if(result == BehaviorState::running)
        {
            reset = false;
            return result;
        }
        if(result == BehaviorState::undefined)
        {
            reset = true;
            return result;
        }
    }
    reset = true;
    return BehaviorState::success;
}
