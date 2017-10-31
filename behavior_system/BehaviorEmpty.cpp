//
// Created by draghan on 2017-10-14.
//

#include "BehaviorEmpty.hpp"


BehaviorEmpty::BehaviorEmpty(uint32_t id, IBehavior::ptr parent)
        : IBehavior{parent, id}
{
}

#ifndef __arm__
void BehaviorEmpty::print(std::ostream &stream)
{
    stream << ".\tid = " << id << '\n';
}

#endif

bool BehaviorEmpty::can_have_children()
{
    return true;
}

std::string BehaviorEmpty::get_glyph()
{
    return "*";
}

BehaviorState BehaviorEmpty::internal_evaluate()
{
    return BehaviorState::undefined;
}
