//
// Created by draghan on 2017-12-02.
//

#include "../external/catch.hpp"
#include "../behavior_system/Decorator/DecoratorInvert.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"

TEST_CASE("Testing DecoratorInvert class", "[Decorator]")
{
    SECTION("All simple cases")
    {
        DecoratorInvert invert_success{0};
        DecoratorInvert invert_failure{0};
        DecoratorInvert invert_undef{0};
        DecoratorInvert invert_running{0};
        BehaviorAction success{1, []()
        {
            return BehaviorState::success;
        }};
        BehaviorAction failure{1, []()
        {
            return BehaviorState::failure;
        }};
        BehaviorAction undef{1, []()
        {
            return BehaviorState::undefined;
        }};
        BehaviorAction running{1, []()
        {
            return BehaviorState::running;
        }};
        invert_failure.add_child(&failure);
        invert_success.add_child(&success);
        invert_running.add_child(&running);
        invert_undef.add_child(&undef);

        REQUIRE(invert_failure.get_number_of_children() == 1);
        REQUIRE(invert_success.get_number_of_children() == 1);
        REQUIRE(invert_running.get_number_of_children() == 1);
        REQUIRE(invert_undef.get_number_of_children() == 1);

        REQUIRE(success.evaluate() == BehaviorState::success);
        REQUIRE(failure.evaluate() == BehaviorState::failure);
        REQUIRE(undef.evaluate() == BehaviorState::undefined);
        REQUIRE(running.evaluate() == BehaviorState::running);

        REQUIRE(invert_success.evaluate() == BehaviorState::failure);
        REQUIRE(invert_failure.evaluate() == BehaviorState::success);
        REQUIRE(invert_undef.evaluate() == BehaviorState::undefined);
        REQUIRE(invert_running.evaluate() == BehaviorState::running);
    }
}