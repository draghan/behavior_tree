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