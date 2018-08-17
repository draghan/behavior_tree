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
// Created by draghan on 2017-10-15.
//

#pragma once

#include <algorithm>
#include <limits>
#include "../IBehavior.hpp"
#include "../composite/BehaviorSelector.hpp"
#include "../composite/BehaviorSequence.hpp"
#include "../primitive/BehaviorAction.hpp"
#include "../primitive/BehaviorCondition.hpp"


class BehaviorTree
{
public:
    using id_t = uint32_t;
    const static id_t undefined_id;

    explicit BehaviorTree(IBehavior::ptr root);
    BehaviorTree();
    BehaviorTree(const BehaviorTree &) = delete;
    BehaviorTree(BehaviorTree &&bt) noexcept;
    ~BehaviorTree();
    BehaviorTree &operator=(const BehaviorTree &bt) = delete;
    BehaviorTree &operator=(BehaviorTree &&bt) noexcept;
    BehaviorTree &operator+=(BehaviorTree &&bt) noexcept;


    template <typename... Args>
    bool set_at_relatively(const Args &... args);
    bool set_at_relatively();
    template <typename... Args>
    bool set_at_absolutely(const Args &... args);
    bool set_at_absolutely();
    bool set_at_id(id_t wanted_id);

    IBehavior::ptr get();
    id_t get_id() const;
    id_t get_node_count() const;


    bool add_sequence();
    bool add_selector();
    bool add_action(BehaviorAction::action_t action);
    bool add_action(BehaviorAction::action_t &&action);
    bool add_condition(BehaviorCondition::predicate_t predicate);
    bool add_condition(BehaviorCondition::predicate_t &&predicate);
    bool add_link(BehaviorTree *link);
    bool add_invert();
    bool add_loop(uint32_t times);
    bool add_max_N_tries(uint32_t tries);

    BehaviorState evaluate();

#ifndef __arm__
    void print(std::ostream &stream);
#endif
    void clear();
private:
    static const id_t id_any;
    std::vector<IBehavior::ptr> nodes;
    IBehavior::ptr root;
    IBehavior::ptr active;
    id_t last_id;
    bool operation_correct;

    IBehavior::ptr get_last_node();
    void go_to_node_relatively(size_t x);
    void back_to_root();
    void delete_nodes();
    bool add_child(IBehavior::ptr &&child);
};

template <typename... Args>
bool BehaviorTree::set_at_relatively(const Args &... args)
{
    if(active == nullptr)
    {
        operation_correct = false;
        return false;
    }

    operation_correct = true;
    std::initializer_list<int>{(go_to_node_relatively(args), 0)...};
    return operation_correct;
}

template <typename... Args>
bool BehaviorTree::set_at_absolutely(const Args &... args)
{
    back_to_root();
    return set_at_relatively(args...);
}
