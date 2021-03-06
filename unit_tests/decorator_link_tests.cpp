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
// Created by draghan on 2017-10-29.
//

#include "../external/catch/single_include/catch2/catch.hpp"
#include "../behavior_system/tree/BehaviorTree.hpp"
#include "../behavior_system/decorator/DecoratorLink.hpp"

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