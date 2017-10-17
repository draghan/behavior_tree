//
// Created by draghan on 2017-10-14.
//

#include "../external/catch.hpp"
#include "../behavior_system/primitive/BehaviorCondition.hpp"

TEST_CASE("Testing BehaviorCondition class", "[Condition]")
{
    SECTION("Always true")
    {
        BehaviorCondition condition_always_true(0, []()
        { return true; });
        REQUIRE(condition_always_true.get_id() == 0);
        REQUIRE(condition_always_true.evaluate() == BehaviorState::success);
        REQUIRE(condition_always_true.evaluate() == BehaviorState::success);
        SECTION("Tests for trying to add children")
        {
            REQUIRE(condition_always_true.can_have_children() == false);
            REQUIRE(condition_always_true.add_child(new BehaviorCondition(0, []() { return true; })) == false);
            REQUIRE(condition_always_true.get_number_of_children() == 0);
        }
    }
    SECTION("Always false")
    {
        BehaviorCondition condition_always_false(1, []()
        { return false; });
        REQUIRE(condition_always_false.get_id() == 1);
        REQUIRE(condition_always_false.evaluate() == BehaviorState::failure);
        REQUIRE(condition_always_false.evaluate() == BehaviorState::failure);
    }
    SECTION("true/false alternatively")
    {
        BehaviorCondition alternatively{2, []()
        {
            static bool tick = false;
            tick = !tick;
            return tick;
        }};
        REQUIRE(alternatively.get_id() == 2);

        REQUIRE(alternatively.evaluate() == BehaviorState::success);
        REQUIRE(alternatively.get_status() == BehaviorState::success);

        REQUIRE(alternatively.evaluate() == BehaviorState::failure);
        REQUIRE(alternatively.get_status() == BehaviorState::failure);

        REQUIRE(alternatively.evaluate() == BehaviorState::success);
        REQUIRE(alternatively.get_status() == BehaviorState::success);

        REQUIRE(alternatively.evaluate() == BehaviorState::failure);
        REQUIRE(alternatively.get_status() == BehaviorState::failure);
    }
}