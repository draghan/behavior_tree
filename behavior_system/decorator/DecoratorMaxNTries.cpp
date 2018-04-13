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

#include "DecoratorMaxNTries.hpp"

DecoratorMaxNTries::DecoratorMaxNTries(uint32_t repeat, uint32_t id, IBehavior::ptr parent)
        : IBehavior{parent, id},
          max{repeat},
          counter{0},
          reset{true}
{
}

std::string DecoratorMaxNTries::get_glyph()
{
    return "< d > <=N";
}

bool DecoratorMaxNTries::can_have_children()
{
    return true;
}

BehaviorState DecoratorMaxNTries::internal_evaluate(id_t id)
{
    if(children.empty())
    {
        return BehaviorState::undefined;
    }
    if(reset)
    {
        counter = 0;
    }

    for(; counter < max; ++counter)
    {
        auto result = get_child_for_eval(0)->evaluate();
        if(result == BehaviorState::success)
        {
            reset = true;
            return result;
        }
        if(result == BehaviorState::running)
        {
            reset = false;
            return result;
        }
        if(result == BehaviorState::undefined)
        {
            reset = true;
            return result;
        }
    }
    reset = true;
    return BehaviorState::failure;
}
