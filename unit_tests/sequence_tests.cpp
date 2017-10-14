//
// Created by draghan on 2017-10-14.
//

#include "../external/catch.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"
#include "../behavior_system/composite/BehaviorSequence.hpp"

TEST_CASE("Testing BehaviorSequence class", "[Sequence]")
{
    SECTION("Testing empty sequence")
    {
        BehaviorSequence sequence{0};
        REQUIRE(sequence.get_status() == BehaviorState::undefined);
        REQUIRE(sequence.evaluate() == BehaviorState::undefined);
    }

    SECTION("Testing with {success, success} children")
    {
        int counter = 0;
        BehaviorAction always_success{1, [&]()
        {
            ++counter;
            return BehaviorState::success;
        }};

        BehaviorSequence sequence{0};
        REQUIRE(sequence.add_child(&always_success) == true);
        REQUIRE(sequence.add_child(&always_success) == true);
        REQUIRE(sequence.get_number_of_children() == 2);
        REQUIRE(sequence.evaluate() == BehaviorState::success);
        REQUIRE(counter == 2);
    }

    SECTION("Testing with {running} children")
    {
        int counter = 0;
        BehaviorAction always_running{1, [&]()
        {
            ++counter;
            return BehaviorState::running;
        }};

        BehaviorSequence sequence{0};
        REQUIRE(sequence.add_child(&always_running) == true);
        REQUIRE(sequence.get_number_of_children() == 1);
        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter == 1);
        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter == 2);
    }

    SECTION("Testing with {success, fail} children")
    {
        int counter = 0;
        BehaviorAction always_success{1, [&]()
        {
            ++counter;
            return BehaviorState::success;
        }};

        BehaviorAction always_fail{2, [&]()
        {
            ++counter;
            return BehaviorState::failure;
        }};

        BehaviorSequence sequence{0};
        REQUIRE(sequence.add_child(&always_success) == true);
        REQUIRE(sequence.add_child(&always_fail) == true);
        REQUIRE(sequence.get_number_of_children() == 2);
        REQUIRE(sequence.evaluate() == BehaviorState::failure);
        REQUIRE(counter == 2);
    }

    SECTION("Testing with {fail, success} children")
    {
        int counter = 0;
        BehaviorAction always_success{1, [&]()
        {
            ++counter;
            return BehaviorState::success;
        }};

        BehaviorAction always_fail{2, [&]()
        {
            ++counter;
            return BehaviorState::failure;
        }};

        BehaviorSequence sequence{0};
        REQUIRE(sequence.add_child(&always_fail) == true);
        REQUIRE(sequence.add_child(&always_success) == true);
        REQUIRE(sequence.get_number_of_children() == 2);
        REQUIRE(sequence.evaluate() == BehaviorState::failure);
        REQUIRE(counter == 1);
    }

    SECTION("Testing with {success, fail/success} children")
    {
        int counter = 0;
        BehaviorAction always_success{1, [&]()
        {
            ++counter;
            return BehaviorState::success;
        }};

        BehaviorAction fail_success{2, [&]()
        {
            static bool success = true;
            success = !success;
            ++counter;
            if(success)
            {
                return BehaviorState::success;
            }
            else
            {
                return BehaviorState::failure;
            }
        }};

        BehaviorSequence sequence{0};
        REQUIRE(sequence.add_child(&always_success) == true);
        REQUIRE(sequence.add_child(&fail_success) == true);

        REQUIRE(sequence.evaluate() == BehaviorState::failure);
        REQUIRE(counter == 2);

        REQUIRE(sequence.evaluate() == BehaviorState::success);
        REQUIRE(counter == 4);

        REQUIRE(sequence.evaluate() == BehaviorState::failure);
        REQUIRE(counter == 6);
    }

    SECTION("Testing with {running/success, success} children")
    {
        int counter = 0;
        BehaviorAction always_success{1, [&]()
        {
            ++counter;
            return BehaviorState::success;
        }};

        BehaviorAction running_success{2, [&]()
        {
            static bool success = true;
            success = !success;
            ++counter;
            if(success)
            {
                return BehaviorState::success;
            }
            else
            {
                return BehaviorState::running;
            }
        }};

        BehaviorSequence sequence{0};
        REQUIRE(sequence.add_child(&always_success) == true);
        REQUIRE(sequence.add_child(&running_success) == true);

        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter == 2);

        REQUIRE(sequence.evaluate() == BehaviorState::success);
        REQUIRE(counter == 4);

        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter == 6);
    }

    SECTION("Testing with {success, running/success, success, failure/success}")
    {
        /*
         * with expected result:
         * (1) {success, running}
         * (2) {success, success, success, failure}
         * (3) {success, running}
         * (4) {success, success, success, success}
         */
        int counter_success = 0;
        int counter_running = 0;
        int counter_failure = 0;
        BehaviorAction success{1, [&]()
        {
            ++counter_success;
            return BehaviorState::success;
        }};

        BehaviorAction running_success{2, [&]()
        {
            static bool success = true;
            success = !success;

            if(success)
            {
                ++counter_success;
                return BehaviorState::success;
            }
            else
            {
                ++counter_running;
                return BehaviorState::running;
            }
        }};

        BehaviorAction failure_success{3, [&]()
        {
            static bool failure = false;
            failure = !failure;
            if(failure)
            {
                ++counter_failure;
                return BehaviorState::failure;
            }
            else
            {
                ++counter_success;
                return BehaviorState::success;
            }
        }};

        BehaviorSequence sequence{0};
        REQUIRE(sequence.add_child(&success) == true);
        REQUIRE(sequence.add_child(&running_success) == true);
        REQUIRE(sequence.add_child(&success) == true);
        REQUIRE(sequence.add_child(&failure_success) == true);
        REQUIRE(sequence.get_number_of_children() == 4);
        REQUIRE(sequence.get_status() == BehaviorState::undefined);

        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter_failure == 0);
        REQUIRE(counter_running == 1);
        REQUIRE(counter_success == 1);

        REQUIRE(sequence.evaluate() == BehaviorState::failure);
        REQUIRE(counter_failure == 1);
        REQUIRE(counter_running == 1);
        REQUIRE(counter_success == 4);

        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter_failure == 1);
        REQUIRE(counter_running == 2);
        REQUIRE(counter_success == 5);

        REQUIRE(sequence.evaluate() == BehaviorState::success);
        REQUIRE(counter_failure == 1);
        REQUIRE(counter_running == 2);
        REQUIRE(counter_success == 9);
    }
}