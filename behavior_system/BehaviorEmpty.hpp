//
// Created by draghan on 2017-10-14.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include "IBehavior.hpp"

class BehaviorEmpty :public IBehavior
{
public:
    ~BehaviorEmpty() override = default;
    explicit BehaviorEmpty(uint32_t id, IBehavior::ptr parent = nullptr);
    bool can_have_children() override;
    std::string get_glyph() override;
    void print(std::ostream &stream) override;
protected:
    BehaviorState internal_evaluate() override;
};



