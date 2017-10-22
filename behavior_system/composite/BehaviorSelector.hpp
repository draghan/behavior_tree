//
// Created by draghan on 2017-10-14.
//

#pragma once


#include "../IBehavior.hpp"

/*
 * todo Consider if composites (selector, sequence) should do reevaluate from scratch in next clock-tick
 * when some of children returns running state or it should remember which child is running and returns immediately to it.
 * */

class BehaviorSelector: public IBehavior
{
public:
    explicit BehaviorSelector(uint32_t id, IBehavior::ptr parent = nullptr);
    ~BehaviorSelector() override = default;

    bool can_have_children() override;
    void print(std::ostream &stream) override;
    std::string get_glyph() override;

protected:
    BehaviorState internal_evaluate() override;
};



