//
// Created by draghan on 2017-10-14.
//

#pragma once

#include "../IBehavior.hpp"

class BehaviorSequence: public IBehavior
{
public:
    explicit BehaviorSequence(uint32_t id, IBehavior::ptr parent = nullptr);
    ~BehaviorSequence() override = default;

    bool can_have_children() override;
    void print(std::ostream &stream) override;
    std::string get_glyph() override;

protected:
    BehaviorState internal_evaluate() override;
};



