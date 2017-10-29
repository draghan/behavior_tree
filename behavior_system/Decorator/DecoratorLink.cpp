//
// Created by draghan on 2017-10-29.
//

#include "DecoratorLink.hpp"

std::string DecoratorLink::get_glyph()
{
    return "d( @ )";
}

bool DecoratorLink::can_have_children()
{
    return false;
}

BehaviorState DecoratorLink::internal_evaluate()
{
    if(link == nullptr)
    {
        return BehaviorState::undefined;
    }
    link->set_at_absolutely();
    return link->evaluate();
}

DecoratorLink::DecoratorLink(BehaviorTree *link, uint32_t id, IBehavior::ptr parent)
        : IBehavior(parent, id),
          link{link}
{
}
