//
// Created by draghan on 2017-10-14.
//

#include "BehaviorAction.hpp"

bool BehaviorAction::can_have_children()
{
    return false;
}

BehaviorAction::BehaviorAction(uint32_t id, BehaviorAction::action_t action, IBehavior::ptr parent):
        IBehavior(parent, id),
        action{std::move(action)}
{
}

void BehaviorAction::print(std::ostream &stream)
{
    stream << "[ ]\tid = " << id << '\n';
}

std::string BehaviorAction::get_glyph()
{
    return "[   ]";
}

BehaviorState BehaviorAction::internal_evaluate()
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
