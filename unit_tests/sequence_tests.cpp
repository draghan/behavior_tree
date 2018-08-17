/*
    This file is distributed under MIT License.

    Copyright (c) 2018 draghan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

//
// Created by draghan on 2017-10-14.
//

#include "../external/catch/single_include/catch2/catch.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"
#include "../behavior_system/composite/BehaviorSequence.hpp"

TEST_CASE("Testing BehaviorSequence class", "[Sequence]")
{
    struct eval_result
    {
        int failure;
        int running;
        int success;
        BehaviorState xstate;
    };
    int counter_failure = 0;
    int counter_success = 0;
    int counter_running = 0;
    auto zero_counters = [&]()
    {
        counter_failure = counter_running = counter_success = 0;
    };
    BehaviorAction fail{1, [&]()
    {
        ++counter_failure;
        return BehaviorState::failure;
    }};

    BehaviorAction success{2, [&]()
    {
        ++counter_success;
        return BehaviorState::success;
    }};

    BehaviorAction running{3, [&]()
    {
        ++counter_running;
        return BehaviorState::running;
    }};

    SECTION("Testing empty sequence")
    {
        BehaviorSequence sequence{0};
        REQUIRE(sequence.get_status() == BehaviorState::undefined);
        REQUIRE(sequence.evaluate() == BehaviorState::undefined);
    }

    SECTION("Testing every 2 elements' combination of from {fail, running, success}")
    {
        SECTION("Testing with {run, run} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 1;
            e1.success = 0;
            e1.xstate = BehaviorState::running;

            e2 = e1;
            e2.running++;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&running) == true);
            REQUIRE(sequence.add_child(&running) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {run, success} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 1;
            e1.success = 0;
            e1.xstate = BehaviorState::running;

            e2 = e1;
            e2.running++;


            BehaviorSequence sequence{0};
            REQUIRE(sequence.can_have_children() == true);
            REQUIRE(sequence.add_child(&running) == true);
            REQUIRE(sequence.add_child(&success) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {run, fail} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 1;
            e1.success = 0;
            e1.xstate = BehaviorState::running;

            e2 = e1;
            e2.running++;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&running) == true);
            REQUIRE(sequence.add_child(&fail) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {success, run} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 1;
            e1.success = 1;
            e1.xstate = BehaviorState::running;

            e2 = e1;
            //e2.success++;
            e2.running++;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&success) == true);
            REQUIRE(sequence.add_child(&running) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {success, success} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 0;
            e1.success = 2;
            e1.xstate = BehaviorState::success;

            e2 = e1;
            e2.success += 2;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&success) == true);
            REQUIRE(sequence.add_child(&success) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {success, fail} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 1;
            e1.running = 0;
            e1.success = 1;
            e1.xstate = BehaviorState::failure;

            e2 = e1;
            e2.failure++;
            e2.success++;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&success) == true);
            REQUIRE(sequence.add_child(&fail) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {fail, run} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 1;
            e1.running = 0;
            e1.success = 0;
            e1.xstate = BehaviorState::failure;

            e2 = e1;
            e2.failure++;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&fail) == true);
            REQUIRE(sequence.add_child(&running) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {fail, success} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 1;
            e1.running = 0;
            e1.success = 0;
            e1.xstate = BehaviorState::failure;

            e2 = e1;
            e2.failure++;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&fail) == true);
            REQUIRE(sequence.add_child(&success) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
        SECTION("Testing with {fail, fail} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 1;
            e1.running = 0;
            e1.success = 0;
            e1.xstate = BehaviorState::failure;

            e2 = e1;
            e2.failure++;

            BehaviorSequence sequence{0};
            REQUIRE(sequence.add_child(&fail) == true);
            REQUIRE(sequence.add_child(&fail) == true);
            REQUIRE(sequence.get_number_of_children() == 2);

            REQUIRE(sequence.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(sequence.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
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
        REQUIRE(counter == 3);

        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter == 5);
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
        REQUIRE(counter_success == 3);

        REQUIRE(sequence.evaluate() == BehaviorState::running);
        REQUIRE(counter_failure == 1);
        REQUIRE(counter_running == 2);
        REQUIRE(counter_success == 4);

        REQUIRE(sequence.evaluate() == BehaviorState::success);
        REQUIRE(counter_failure == 1);
        REQUIRE(counter_running == 2);
        REQUIRE(counter_success == 7);
    }
}