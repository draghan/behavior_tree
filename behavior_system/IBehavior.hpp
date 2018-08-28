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

#pragma once

#include <string>
#include <vector>

#ifndef __arm__
#include <ostream>
#endif

enum class BehaviorState: uint8_t
{
    undefined,
    success,
    failure,
    running
};

class IBehavior
{
public:
    using ptr = IBehavior *;
    using id_t = uint32_t;

    const static id_t undefined_id;

#ifndef __arm__

    // Decided to disable printing to the streams on ARM processors
    // in order to keep some bytes of compiled result file.
    // With stream library included, result files were unacceptable
    // oversized for Cortex M.

    void print_family(std::string indent, bool last, std::ostream &stream);
    void introduce_yourself(std::ostream &stream);

#endif

    id_t get_id() const;
    void set_id(id_t id);

    bool add_child(ptr child);
    size_t get_number_of_children() const;

    ptr get_child(size_t index) const;
    ptr get_last_child() const;

    ptr get_parent() const;

    explicit IBehavior(ptr parent, uint32_t id = 0);
    IBehavior(const IBehavior&) = delete;
    void operator=(const IBehavior&) = delete;
    virtual ~IBehavior() = default;

    BehaviorState evaluate();
    BehaviorState get_status() const;

    bool operator==(const IBehavior &);

    virtual std::string get_glyph() const;
    virtual bool can_have_children() const = 0;

protected:
    std::vector<ptr> children;
    ptr parent;
    id_t id;
    BehaviorState status;
    id_t last_evaluated_child;

    ptr get_child_for_eval(id_t id);
    id_t get_last_evaluated_child_id();

    virtual BehaviorState internal_evaluate(id_t id_child_for_evaluation) = 0;
    BehaviorState internal_evaluate();
};



