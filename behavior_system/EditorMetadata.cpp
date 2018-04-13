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
// Created by draghan on 2017-10-21.
//

#include "EditorMetadata.hpp"
#include "IBehavior.hpp"

#ifndef __arm__

EditorMetadata::EditorMetadata(class IBehavior *owner)
        : x{0},
          y{0},
          mod{0.0f},
          width{0.0f},
          height{0.0f},
          owner{owner}
{
}

std::vector<IBehavior *> EditorMetadata::children() const
{
    if(owner == nullptr)
    {
        return std::vector<IBehavior *>{};
    }
    return owner->children;
}

bool EditorMetadata::is_leaf() const
{
    return owner->children.empty();
}

bool EditorMetadata::is_left_most() const
{
    if(owner->parent == nullptr)
    {
        return true;
    }
    return (get_left_most_sibling()) == owner;
}

IBehavior *EditorMetadata::get_previous_sibling() const
{
    if(owner->parent == nullptr)
    {
        return nullptr;
    }
    size_t id_found;
    size_t id_last = owner->parent->children.size();
    auto &siblings = owner->parent->children;
    for(id_found = 0; id_found < id_last; ++id_found)
    {
        if(siblings[id_found]->id == owner->id)
        {
            break;
        }
    }
    if(id_found == id_last || id_found == 0)
    {
        return nullptr;
    }
    else
    {
        return siblings[id_found - 1];
    }
}

IBehavior *EditorMetadata::get_next_sibling() const
{
    if(owner->parent == nullptr)
    {
        return nullptr;
    }
    size_t id_found;
    size_t id_last = owner->parent->children.size();
    auto &siblings = owner->parent->children;
    for(id_found = 0; id_found < id_last; ++id_found)
    {
        if(siblings[id_found]->id == owner->id)
        {
            break;
        }
    }
    if(id_found == id_last || id_found == id_last - 1)
    {
        return nullptr;
    }
    else
    {
        return siblings[id_found + 1];
    }
}

IBehavior *EditorMetadata::get_left_most_sibling() const
{
    if(owner->parent == nullptr)
    {
        return nullptr;
    }
    auto &siblings = owner->parent->children;
    if(siblings.empty())
    {
        return nullptr;
    }
    else
    {
        return siblings[0];
    }
}

IBehavior *EditorMetadata::get_left_most_child() const
{
    if(owner->children.empty())
    {
        return nullptr;
    }
    else
    {
        return owner->children[0];
    }
}

IBehavior *EditorMetadata::get_right_most_child() const
{
    if(owner->children.empty())
    {
        return nullptr;
    }
    else
    {
        return owner->children[owner->children.size() - 1];
    }
}

IBehavior *EditorMetadata::parent() const
{
    return owner->parent;
}

#endif