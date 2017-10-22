//
// Created by draghan on 2017-10-21.
//

#pragma once
#ifndef __arm__

#include <vector>

class IBehavior;

class DrawHelper
{
public:
    explicit DrawHelper(class IBehavior *owner);

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
private:
    IBehavior *owner;
};

#endif