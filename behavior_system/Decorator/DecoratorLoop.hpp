//
// Created by draghan on 2017-12-03.
//

#pragma once


#include "../IBehavior.hpp"

class DecoratorLoop: public IBehavior
{
public:
    explicit DecoratorLoop(uint32_t repeat, uint32_t id, IBehavior::ptr parent = nullptr);
    std::string get_glyph() override;
    bool can_have_children() override;
protected:
    BehaviorState internal_evaluate(id_t id) override;
private:
    uint32_t max;
    uint32_t counter;
    bool reset;
};



