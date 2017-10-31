//
// Created by draghan on 2017-10-14.
//

#pragma once


#include <functional>
#include "../IBehavior.hpp"

class BehaviorCondition: public IBehavior
{
public:
    using predicate_t = std::function<bool()>;

    BehaviorCondition(uint32_t id, predicate_t predicate, IBehavior::ptr parent = nullptr);
    ~BehaviorCondition() override = default;

    bool can_have_children() override;
#ifndef __arm__
    void print(std::ostream &stream) override;
#endif
    std::string get_glyph() override;
protected:
    predicate_t predicate;
    BehaviorState internal_evaluate() override;
};



