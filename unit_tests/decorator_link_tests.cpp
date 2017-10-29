//
// Created by draghan on 2017-10-29.
//

#include "../external/catch.hpp"
#include "../behavior_system/tree/BehaviorTree.hpp"
#include "../behavior_system/Decorator/DecoratorLink.hpp"

TEST_CASE("Testing DecoratorLink class", "[Decorator]")
{
    SECTION("Simple tree")
    {
        BehaviorTree simple;
        simple.add_action([]()
                          {
                              return BehaviorState::success;
                          });
        DecoratorLink link(&simple, 0);

        REQUIRE(link.get_number_of_children() == 0);
        REQUIRE(link.evaluate() == BehaviorState::success);
    }
}