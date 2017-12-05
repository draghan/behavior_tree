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

#ifndef __arm__
void BehaviorSelector::print(std::ostream &stream)
{
    stream << "[?]\tid = " << id << '\n';
}

#endif

std::string BehaviorSelector::get_glyph()
{
    return "[ ? ]";
}

BehaviorState BehaviorSelector::internal_evaluate(id_t id)
{
    if(children.empty())
    {
        return BehaviorState::undefined;
    }

    for(id_t i = id; i < children.size(); ++i)
    {
        if(get_child_for_eval(i)->evaluate() != BehaviorState::failure)
        {
            return get_child_for_eval(i)->get_status();
        }
    }

//    for(auto &child : children)
//    {
//        if(child->evaluate() != BehaviorState::failure)
//        {
//            return child->get_status();
//        }
//    }
    return BehaviorState::failure;
}
