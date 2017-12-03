//
// Created by draghan on 2017-12-03.
//

#include "../external/catch.hpp"
#include "../behavior_system/Decorator/DecoratorMaxNTries.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"

TEST_CASE("Testing DecoratorMaxNTries class", "[Decorator]")
{
    SECTION("All simple cases")
    {
        DecoratorMaxNTries decorator{5, 0};
        int ctr_success = 0;
        int ctr_failure = 0;
        int ctr = 0;
        BehaviorAction action{1, [&]()
        {
            ++ctr;
            if(ctr == 2)
            {
                ++ctr_success;
                return BehaviorState::success;
            }
            else
            {
                ++ctr_failure;
                return BehaviorState::failure;
            }
        }};
        decorator.add_child(&action);
        REQUIRE(decorator.evaluate() == BehaviorState::success);
        REQUIRE(ctr == 2);
        REQUIRE(ctr_failure == 1);
        REQUIRE(ctr_success == 1);
    }
}