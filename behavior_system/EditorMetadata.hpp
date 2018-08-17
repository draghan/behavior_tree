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

#pragma once

#ifndef __arm__

#include <vector>
#include <string>

class IBehavior;

class EditorMetadata
{
public:
    explicit EditorMetadata(class IBehavior *owner);
    EditorMetadata(const EditorMetadata&) = delete;
    void operator=(const EditorMetadata&) = delete;

    int x;
    int y;
    float mod;
    float width;
    float height;

    bool is_leaf() const;
    bool is_left_most() const;

    IBehavior *get_previous_sibling() const;
    IBehavior *get_next_sibling() const;
    IBehavior *get_left_most_sibling() const;
    IBehavior *get_left_most_child() const;
    IBehavior *get_right_most_child() const;

    IBehavior *parent() const;
    std::vector<IBehavior *> children() const;

    std::string metadata;
    std::string description;
    std::string node_type;
private:
    IBehavior *owner;
};

#endif