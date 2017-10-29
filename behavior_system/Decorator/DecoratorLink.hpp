//
// Created by draghan on 2017-10-29.
//

#pragma once


#include "../IBehavior.hpp"
#include "../tree/BehaviorTree.hpp"

class DecoratorLink: public IBehavior
{
public:
    explicit DecoratorLink(BehaviorTree *link, uint32_t id, IBehavior::ptr parent = nullptr);
    std::string get_glyph() override;
    bool can_have_children() override;
protected:
    BehaviorState internal_evaluate() override;
private:
    BehaviorTree *link;
};



