#include <utility>

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

#include "BehaviorCondition.hpp"

bool BehaviorCondition::can_have_children()
{
    return false;
}

std::string BehaviorCondition::get_glyph()
{
    return "(   )";
}

BehaviorCondition::BehaviorCondition(uint32_t id, BehaviorCondition::predicate_t predicate, IBehavior::ptr parent)
        : IBehavior(parent, id),
          predicate{std::move(predicate)}
{
}

BehaviorState BehaviorCondition::internal_evaluate(id_t)
{
    if(!predicate)
    {
        return BehaviorState::undefined;
    }

    if(predicate())
    {
        return BehaviorState::success;
    }
    else
    {
        return BehaviorState::failure;
    }
}
