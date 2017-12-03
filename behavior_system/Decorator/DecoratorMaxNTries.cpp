//
// Created by draghan on 2017-12-03.
//

#include "DecoratorMaxNTries.hpp"

DecoratorMaxNTries::DecoratorMaxNTries(uint32_t repeat, uint32_t id, IBehavior::ptr parent)
        : IBehavior{parent, id},
          max{repeat},
          counter{0},
          reset{true}
{
}

std::string DecoratorMaxNTries::get_glyph()
{
    return "< d > <=N";
}

bool DecoratorMaxNTries::can_have_children()
{
    return true;
}

BehaviorState DecoratorMaxNTries::internal_evaluate()
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
        auto result = children[0]->evaluate();
        if(result == BehaviorState::success)
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
    return BehaviorState::failure;
}
