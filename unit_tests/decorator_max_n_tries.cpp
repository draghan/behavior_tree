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

#include "../external/catch/single_include/catch2/catch.hpp"
#include "../behavior_system/decorator/DecoratorMaxNTries.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"

TEST_CASE("Testing DecoratorMaxNTries class", "[Decorator]")
{
    SECTION("All simple cases")
    {
        DecoratorMaxNTries decorator{5, 0};
        int ctr_success = 0;
        int ctr_failure = 0;
        int ctr = 0;
        BehaviorAction action{1, [&]()
        {
            ++ctr;
            if(ctr == 2)
            {
                ++ctr_success;
                return BehaviorState::success;
            }
            else
            {
                ++ctr_failure;
                return BehaviorState::failure;
            }
        }};
        decorator.add_child(&action);
        REQUIRE(decorator.evaluate() == BehaviorState::success);
        REQUIRE(ctr == 2);
        REQUIRE(ctr_failure == 1);
        REQUIRE(ctr_success == 1);
    }
}