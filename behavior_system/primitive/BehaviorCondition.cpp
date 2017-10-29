//
// Created by draghan on 2017-10-14.
//

#include "BehaviorCondition.hpp"

bool BehaviorCondition::can_have_children()
{
    return false;
}

void BehaviorCondition::print(std::ostream &stream)
{
    stream << "( )\tid = " << id << '\n';
}

std::string BehaviorCondition::get_glyph()
{
    return "(   )";
}

BehaviorCondition::BehaviorCondition(uint32_t id, BehaviorCondition::predicate_t predicate, IBehavior::ptr parent)
        : IBehavior(parent, id),
          predicate{std::move(predicate)}
{
}

BehaviorState BehaviorCondition::internal_evaluate()
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
