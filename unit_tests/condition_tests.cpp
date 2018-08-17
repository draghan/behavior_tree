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
#include "../behavior_system/primitive/BehaviorCondition.hpp"

TEST_CASE("Testing BehaviorCondition class", "[Condition]")
{
    SECTION("Always true")
    {
        BehaviorCondition condition_always_true(0, []()
        { return true; });
        REQUIRE(condition_always_true.get_id() == 0);
        REQUIRE(condition_always_true.evaluate() == BehaviorState::success);
        REQUIRE(condition_always_true.evaluate() == BehaviorState::success);
        SECTION("Tests for trying to add children")
        {
            REQUIRE(condition_always_true.can_have_children() == false);
            REQUIRE(condition_always_true.add_child(new BehaviorCondition(0, []() { return true; })) == false);
            REQUIRE(condition_always_true.get_number_of_children() == 0);
        }
    }
    SECTION("Always false")
    {
        BehaviorCondition condition_always_false(1, []()
        { return false; });
        REQUIRE(condition_always_false.get_id() == 1);
        REQUIRE(condition_always_false.evaluate() == BehaviorState::failure);
        REQUIRE(condition_always_false.evaluate() == BehaviorState::failure);
    }
    SECTION("true/false alternatively")
    {
        BehaviorCondition alternatively{2, []()
        {
            static bool tick = false;
            tick = !tick;
            return tick;
        }};
        REQUIRE(alternatively.get_id() == 2);

        REQUIRE(alternatively.evaluate() == BehaviorState::success);
        REQUIRE(alternatively.get_status() == BehaviorState::success);

        REQUIRE(alternatively.evaluate() == BehaviorState::failure);
        REQUIRE(alternatively.get_status() == BehaviorState::failure);

        REQUIRE(alternatively.evaluate() == BehaviorState::success);
        REQUIRE(alternatively.get_status() == BehaviorState::success);

        REQUIRE(alternatively.evaluate() == BehaviorState::failure);
        REQUIRE(alternatively.get_status() == BehaviorState::failure);
    }
}