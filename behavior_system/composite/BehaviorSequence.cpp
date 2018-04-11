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

#ifndef __arm__
void BehaviorSequence::print(std::ostream &stream)
{
    stream << "[->]\tid = " << id << '\n';
}

#endif

std::string BehaviorSequence::get_glyph()
{
    return "[->]";
}

BehaviorState BehaviorSequence::internal_evaluate(id_t id)
{
    if(children.empty())
    {
        return BehaviorState::undefined;
    }

    for(id_t i = id; i < children.size(); ++i)
    {
        if(get_child_for_eval(i)->evaluate() != BehaviorState::success)
        {
            return get_child_for_eval(i)->get_status();
        }
    }
    return BehaviorState::success;
}
