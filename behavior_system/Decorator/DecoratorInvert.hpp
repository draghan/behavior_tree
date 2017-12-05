//
// Created by draghan on 2017-12-02.
//

#pragma once


#include "../IBehavior.hpp"

class DecoratorInvert: public IBehavior
{
public:
    explicit DecoratorInvert(uint32_t id, IBehavior::ptr parent = nullptr);
    std::string get_glyph() override;
    bool can_have_children() override;
protected:
    BehaviorState internal_evaluate(id_t id) override;
};



