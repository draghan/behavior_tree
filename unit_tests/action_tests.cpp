//
// Created by draghan on 2017-10-14.
//

#include "../external/catch.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"

TEST_CASE("Testing BehaviorAction class", "[Action]")
{
    SECTION("Test single action")
    {
        BehaviorAction root{10, []()
        {
            return BehaviorState::success;
        }};

        REQUIRE(root.get_id() == 10);
        REQUIRE(root.get_number_of_children() == 0);
        REQUIRE(root.get_child(0) == nullptr);
        REQUIRE(root.evaluate() == BehaviorState::success);
        SECTION("Tests for trying to add children")
        {
            REQUIRE(root.can_have_children() == false);
            REQUIRE(root.add_child(new BehaviorAction(0, []() { return BehaviorState::running; })) == false);
            REQUIRE(root.get_number_of_children() == 0);
        }
    }

    SECTION("Testing managing of captured objects in action()")
    {
        int x = 0;
        BehaviorAction action_increment_x_once{14, [&]()
        {
            static bool incremented = false;
            if(incremented)
            {
                return BehaviorState::failure;
            }
            else
            {
                ++x;
                incremented = true;
                return BehaviorState::success;
            }
        }};


    }
}