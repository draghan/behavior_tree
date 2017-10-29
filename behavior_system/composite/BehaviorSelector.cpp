//
// Created by draghan on 2017-10-14.
//

#include "BehaviorSelector.hpp"

bool BehaviorSelector::can_have_children()
{
    return true;
}

BehaviorSelector::BehaviorSelector(uint32_t id, IBehavior::ptr parent)
        : IBehavior{parent, id}
{
}

void BehaviorSelector::print(std::ostream &stream)
{
    stream << "[?]\tid = " << id << '\n';
}

std::string BehaviorSelector::get_glyph()
{
    return "[ ? ]";
}

BehaviorState BehaviorSelector::internal_evaluate()
{
    if(children.empty())
    {
        return BehaviorState::undefined;
    }

    for(auto &child : children)
    {
        if(child->evaluate() != BehaviorState::failure)
        {
            return child->get_status();
        }
    }
    return BehaviorState::failure;
}
