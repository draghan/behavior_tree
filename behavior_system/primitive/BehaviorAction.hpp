//
// Created by draghan on 2017-10-14.
//

#pragma once


#include <functional>
#include "../IBehavior.hpp"

class BehaviorAction: public IBehavior
{
public:
    using action_t = std::function<BehaviorState()>;
    BehaviorAction(uint32_t id, action_t action, IBehavior::ptr parent = nullptr);

    bool can_have_children() override;
    void print(std::ostream &stream) override;
    std::string get_glyph() override;

protected:
    BehaviorState internal_evaluate() override;

private:
    action_t action;
};



