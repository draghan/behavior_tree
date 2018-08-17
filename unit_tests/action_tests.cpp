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