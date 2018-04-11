//
// Created by draghan on 2017-10-14.
//

#pragma once


#include "../IBehavior.hpp"

class BehaviorSelector: public IBehavior
{
public:
    explicit BehaviorSelector(uint32_t id, IBehavior::ptr parent = nullptr);
    ~BehaviorSelector() override = default;

    bool can_have_children() override;
#ifndef __arm__
    void print(std::ostream &stream) override;
#endif
    std::string get_glyph() override;

protected:
    BehaviorState internal_evaluate(id_t id) override;
};



