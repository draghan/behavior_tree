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