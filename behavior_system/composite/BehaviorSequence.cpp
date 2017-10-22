//
// Created by draghan on 2017-10-14.
//

#include "BehaviorSequence.hpp"

bool BehaviorSequence::can_have_children()
{
    return true;
}

BehaviorSequence::BehaviorSequence(uint32_t id, IBehavior::ptr parent)
        : IBehavior{parent, id}
{
}

void BehaviorSequence::print(std::ostream &stream)
{
    stream << "[->]\tid = " << id << '\n';
}

std::string BehaviorSequence::get_glyph()
{
    return "[->]";
}

BehaviorState BehaviorSequence::internal_evaluate()
{
    if(children.empty())
    {
        return BehaviorState::undefined;
    }

    for(auto &child : children)
    {
        if(child->evaluate() != BehaviorState::success)
        {
            return child->get_status();
        }
    }
    return BehaviorState::success;
}
