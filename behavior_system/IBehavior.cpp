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

#include "IBehavior.hpp"

#ifndef __arm__

void IBehavior::PrintPretty(std::string indent, bool last, std::ostream &stream) // todo: clean print functions
{
    stream << indent;
    if(last)
    {
        stream << "\\-";
        indent += "  ";
    }
    else
    {
        stream << ("|-");
        indent += "| ";
    }
    print(stream);
    for(size_t i = 0; i < children.size(); i++)
    {
        children[i]->PrintPretty(indent, i == children.size() - 1, stream);
    }
}

void IBehavior::print(std::ostream &stream)
{
    stream << "IBehavior [" << id << "]\n";
}

#endif

IBehavior::id_t IBehavior::get_id() const
{
    return id;
}

void IBehavior::set_id(IBehavior::id_t id)
{
    this->id = id;
}

bool IBehavior::add_child(IBehavior::ptr child)
{
    if(!can_have_children() || child == this)
    {
        return false;
    }

    children.push_back(child);
    return true;
}

size_t IBehavior::get_number_of_children() const
{
    return children.size();
}

IBehavior::ptr IBehavior::get_child(size_t index) const
{
    if(index >= children.size())
    {
        return nullptr;
    }
    return children[index];
}

IBehavior::ptr IBehavior::get_last_child() const
{
    if(children.empty())
    {
        return nullptr;
    }
    return *(children.end() - 1);
}

IBehavior::ptr IBehavior::get_parent() const
{
    return parent;
}

IBehavior::IBehavior(IBehavior::ptr parent, uint32_t id)
        :
#ifndef __arm__
          draw_helper{this},
#endif
          children{},
          parent{parent},
          id{id},
          status{BehaviorState::undefined}
{
}

BehaviorState IBehavior::evaluate()
{
    if(status == BehaviorState::running)
    {
        status = internal_evaluate(get_last_evaluated_child_id());
    }
    else
    {
        status = internal_evaluate();
    }
    return status;
}


BehaviorState IBehavior::get_status() const
{
    return status;
}

bool IBehavior::operator==(const IBehavior &)
{
    return false;
}
