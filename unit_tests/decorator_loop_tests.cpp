//
// Created by draghan on 2017-12-03.
//

#include "../external/catch.hpp"
#include "../behavior_system/Decorator/DecoratorLoop.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"

TEST_CASE("Testing DecoratorLoop class", "[Decorator]")
{
    SECTION("All simple cases")
    {
        DecoratorLoop loop_success{2, 0};
        DecoratorLoop loop_failure{2, 0};
        DecoratorLoop loop_undef{2, 0};
        DecoratorLoop loop_running{2, 0};
        int ctr;
        BehaviorAction success{1, [&]()
        {
            ++ctr;
            return BehaviorState::success;
        }};
        BehaviorAction failure{1, [&]()
        {
            ++ctr;
            return BehaviorState::failure;
        }};
        BehaviorAction undef{1, [&]()
        {
            ++ctr;
            return BehaviorState::undefined;
        }};
        BehaviorAction running{1, [&]()
        {
            ++ctr;
            return BehaviorState::running;
        }};
        loop_failure.add_child(&failure);
        loop_success.add_child(&success);
        loop_running.add_child(&running);
        loop_undef.add_child(&undef);

        REQUIRE(loop_failure.get_number_of_children() == 1);
        REQUIRE(loop_success.get_number_of_children() == 1);
        REQUIRE(loop_running.get_number_of_children() == 1);
        REQUIRE(loop_undef.get_number_of_children() == 1);


        ctr = 0;
        REQUIRE(loop_success.evaluate() == BehaviorState::success);
        REQUIRE(ctr == 2);

        ctr = 0;
        REQUIRE(loop_failure.evaluate() == BehaviorState::failure);
        REQUIRE(ctr == 1);

        ctr = 0;
        REQUIRE(loop_undef.evaluate() == BehaviorState::undefined);
        REQUIRE(ctr == 1);

        ctr = 0;
        REQUIRE(loop_running.evaluate() == BehaviorState::running);
        REQUIRE(ctr == 1);
    }
}