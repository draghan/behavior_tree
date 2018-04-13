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

#include "../external/catch.hpp"
#include "../behavior_system/composite/BehaviorSelector.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"

TEST_CASE("Testing BehaviorSelector class", "[Selector]")
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
    bool f_run_succ__run = false;
    bool f_fail_succ__fail = false;
    auto zero_counters = [&]()
    {
        counter_failure = counter_running = counter_success = 0;
        f_run_succ__run = f_fail_succ__fail = false;
    };

    BehaviorAction running_success{4, [&]()
    {
        f_run_succ__run = !f_run_succ__run;
        if(f_run_succ__run)
        {
            ++counter_running;
            return BehaviorState::running;
        }
        else
        {
            ++counter_success;
            return BehaviorState::success;
        }
    }};

    BehaviorAction fail_success{4, [&]()
    {
        f_fail_succ__fail = !f_fail_succ__fail;
        if(f_fail_succ__fail)
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

    SECTION("Check preconditions on changing state BehaviorStates")
    {
        zero_counters();
        REQUIRE(running_success.evaluate() == BehaviorState::running);
        REQUIRE(running_success.evaluate() == BehaviorState::success);
        REQUIRE(running_success.evaluate() == BehaviorState::running);
        REQUIRE(running_success.evaluate() == BehaviorState::success);

        REQUIRE(fail_success.evaluate() == BehaviorState::failure);
        REQUIRE(fail_success.evaluate() == BehaviorState::success);
        REQUIRE(fail_success.evaluate() == BehaviorState::failure);
        REQUIRE(fail_success.evaluate() == BehaviorState::success);
        zero_counters();
    }

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

    SECTION("Testing empty selector")
    {
        BehaviorSelector selector{0};
        REQUIRE(selector.get_status() == BehaviorState::undefined);
        REQUIRE(selector.evaluate() == BehaviorState::undefined);
        REQUIRE(selector.can_have_children() == true);
    }

    SECTION("Testing every 2 elements' combination of from {fail, running, success}")
    {

        SECTION("Testing with {fail, fail} children")
        {
            zero_counters();
            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&fail) == true);
            REQUIRE(selector.add_child(&fail) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == BehaviorState::failure);
            REQUIRE(counter_failure == 2);
            REQUIRE(counter_running == 0);
            REQUIRE(counter_success == 0);
            REQUIRE(selector.evaluate() == BehaviorState::failure);
            REQUIRE(counter_failure == 4);
            REQUIRE(counter_running == 0);
            REQUIRE(counter_success == 0);
        }

        SECTION("Testing with {success, success} children")
        {
            zero_counters();
            eval_result e1;
            e1.failure = 0;
            e1.running = 0;
            e1.success = 1;
            e1.xstate = BehaviorState::success;

            eval_result e2;
            e2.xstate = BehaviorState::success;
            e2.failure = 0;
            e2.running = 0;
            e2.success = 2;

            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&success) == true);
            REQUIRE(selector.add_child(&success) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }

        SECTION("Testing with {running, running} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 1;
            e1.success = 0;
            e1.xstate = BehaviorState::running;
            e2 = e1;
            e2.running++;
            BehaviorSelector selector{0};

            REQUIRE(selector.add_child(&running) == true);
            REQUIRE(selector.add_child(&running) == true);

            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }

        SECTION("Testing with {success, fail} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 0;
            e1.success = 1;
            e1.xstate = BehaviorState::success;

            e2 = e1;
            e2.success++;

            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&success) == true);
            REQUIRE(selector.add_child(&fail) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
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
            e1.success = 1;
            e1.xstate = BehaviorState::success;

            e2 = e1;
            e2.success++;
            e2.failure++;

            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&fail) == true);
            REQUIRE(selector.add_child(&success) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }

        SECTION("Testing with {running, success} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 1;
            e1.success = 0;
            e1.xstate = BehaviorState::running;

            e2 = e1;
            e2.running++;

            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&running) == true);
            REQUIRE(selector.add_child(&success) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }

        SECTION("Testing with {running, failure} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 1;
            e1.success = 0;
            e1.xstate = BehaviorState::running;

            e2 = e1;
            e2.running++;

            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&running) == true);
            REQUIRE(selector.add_child(&fail) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }

        SECTION("Testing with {success, running} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 0;
            e1.running = 0;
            e1.success = 1;
            e1.xstate = BehaviorState::success;

            e2 = e1;
            e2.success++;

            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&success) == true);
            REQUIRE(selector.add_child(&running) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }

        SECTION("Testing with {failure, running} children")
        {
            zero_counters();
            eval_result e1, e2;
            e1.failure = 1;
            e1.running = 1;
            e1.success = 0;
            e1.xstate = BehaviorState::running;

            e2 = e1;
            e2.running++;

            BehaviorSelector selector{0};
            REQUIRE(selector.add_child(&fail) == true);
            REQUIRE(selector.add_child(&running) == true);
            REQUIRE(selector.get_number_of_children() == 2);

            REQUIRE(selector.evaluate() == e1.xstate);
            REQUIRE(counter_failure == e1.failure);
            REQUIRE(counter_running == e1.running);
            REQUIRE(counter_success == e1.success);

            REQUIRE(selector.evaluate() == e2.xstate);
            REQUIRE(counter_failure == e2.failure);
            REQUIRE(counter_running == e2.running);
            REQUIRE(counter_success == e2.success);
        }
    }

    SECTION("Testing with {fail/success, success} children")
    {
        zero_counters();
        eval_result e1, e2;
        e1.failure = 1;
        e1.running = 0;
        e1.success = 1;
        e1.xstate = BehaviorState::success;
        e2 = e1;
        e2.success++;
        BehaviorSelector selector{0};
        REQUIRE(selector.add_child(&fail_success) == true);
        REQUIRE(selector.add_child(&success) == true);
        REQUIRE(selector.get_number_of_children() == 2);

        REQUIRE(selector.evaluate() == e1.xstate);
        REQUIRE(counter_failure == e1.failure);
        REQUIRE(counter_running == e1.running);
        REQUIRE(counter_success == e1.success);

        REQUIRE(selector.evaluate() == e2.xstate);
        REQUIRE(counter_failure == e2.failure);
        REQUIRE(counter_running == e2.running);
        REQUIRE(counter_success == e2.success);
    }

    SECTION("Testing with {running/success, success} children")
    {
        zero_counters();
        eval_result e1, e2;
        e1.failure = 0;
        e1.running = 1;
        e1.success = 0;
        e1.xstate = BehaviorState::running;
        e2 = e1;
        e2.success++;
        e2.xstate = BehaviorState::success;
        BehaviorSelector selector{0};
        REQUIRE(selector.add_child(&running_success) == true);
        REQUIRE(selector.add_child(&success) == true);
        REQUIRE(selector.get_number_of_children() == 2);

        REQUIRE(selector.evaluate() == e1.xstate);
        REQUIRE(counter_failure == e1.failure);
        REQUIRE(counter_running == e1.running);
        REQUIRE(counter_success == e1.success);

        REQUIRE(selector.evaluate() == e2.xstate);
        REQUIRE(counter_failure == e2.failure);
        REQUIRE(counter_running == e2.running);
        REQUIRE(counter_success == e2.success);
    }

    SECTION("Testing with {fail, failure/success, running/success}")
    {
        zero_counters();
        eval_result e1, e2, e3;
        e1.failure = 2;
        e1.running = 1;
        e1.success = 0;
        e1.xstate = BehaviorState::running;
        e2 = e1;
        //e2.failure++;
        e2.success++;
        e2.xstate = BehaviorState::success;
        e3 = e2;
        e3.failure += 1;
        e3.success++;
        e2.xstate = BehaviorState::success;

        BehaviorSelector selector{0};
        REQUIRE(selector.add_child(&fail) == true);
        REQUIRE(selector.add_child(&fail_success) == true);
        REQUIRE(selector.add_child(&running_success) == true);
        REQUIRE(selector.get_number_of_children() == 3);

        REQUIRE(selector.evaluate() == e1.xstate);
        REQUIRE(counter_failure == e1.failure);
        REQUIRE(counter_running == e1.running);
        REQUIRE(counter_success == e1.success);

        REQUIRE(selector.evaluate() == e2.xstate);
        REQUIRE(counter_failure == e2.failure);
        REQUIRE(counter_running == e2.running);
        REQUIRE(counter_success == e2.success);

        REQUIRE(selector.evaluate() == e3.xstate);
        REQUIRE(counter_failure == e3.failure);
        REQUIRE(counter_running == e3.running);
        REQUIRE(counter_success == e3.success);
    }
}