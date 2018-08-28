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

#include <limits>
#include "IBehavior.hpp"

#ifndef __arm__

const IBehavior::id_t IBehavior::undefined_id{std::numeric_limits<IBehavior::id_t>::max()};

void IBehavior::print_family(std::string indent, bool last, std::ostream &stream)
{
    stream << indent;
    if(last)
    {
        stream << "\\-";
        indent += "  ";
    }
    else
    {
        stream << "|-";
        indent += "| ";
    }

    introduce_yourself(stream);
    for(size_t i = 0; i < children.size(); i++)
    {
        children[i]->print_family(indent, i == children.size() - 1, stream);
    }
}

void IBehavior::introduce_yourself(std::ostream &stream)
{
    stream << get_glyph() << "\tid = " << id << '\n';
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
        : children{},
          parent{parent},
          id{id},
          status{BehaviorState::undefined},
          last_evaluated_child{undefined_id}
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

std::string IBehavior::get_glyph() const
{
    return "|IB|";
}

IBehavior::ptr IBehavior::get_child_for_eval(IBehavior::id_t id)
{
    last_evaluated_child = id;
    if(id >= children.size())
    {
        return nullptr;
    }
    return children[id];
}

IBehavior::id_t IBehavior::get_last_evaluated_child_id()
{
    if(last_evaluated_child >= children.size())
    {
        last_evaluated_child = 0;
    }
    return last_evaluated_child;
}

BehaviorState IBehavior::internal_evaluate()
{
    return internal_evaluate(0);
}
