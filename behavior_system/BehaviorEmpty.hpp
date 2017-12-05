//
// Created by draghan on 2017-10-14.
//

#pragma once

#include <cstdint>
#include <vector>
#include <string>

#ifndef __arm__
#include <iostream>

#endif
#include "IBehavior.hpp"

class BehaviorEmpty :public IBehavior
{
public:
    ~BehaviorEmpty() override = default;
    explicit BehaviorEmpty(uint32_t id, IBehavior::ptr parent = nullptr);
    bool can_have_children() override;
    std::string get_glyph() override;
#ifndef __arm__
    void print(std::ostream &stream) override;
#endif
protected:
    BehaviorState internal_evaluate(id_t id_child) override;
};



