//
// Created by draghan on 2017-10-14.
//

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include "EditorMetadata.hpp"

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

#ifndef __arm__

    friend class EditorMetadata;

    EditorMetadata draw_helper;

    void PrintPretty(std::string indent, bool last, std::ostream &stream);
    virtual void print(std::ostream &stream);
#endif

    id_t get_id() const;
    void set_id(id_t id);

    bool add_child(ptr child);
    size_t get_number_of_children() const;

    ptr get_child(size_t index) const;
    ptr get_last_child() const;

    ptr get_parent() const;

    explicit IBehavior(ptr parent, uint32_t id = 0);
    virtual ~IBehavior() = default;

    BehaviorState evaluate();
    BehaviorState get_status() const;

    bool operator==(const IBehavior &);

    virtual std::string get_glyph() = 0;
    virtual bool can_have_children() = 0;
protected:
    std::vector<ptr> children;
    ptr parent;
    id_t id;
    BehaviorState status;

    virtual BehaviorState internal_evaluate() = 0;
};



