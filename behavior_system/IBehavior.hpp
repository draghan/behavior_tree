//
// Created by draghan on 2017-10-14.
//

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#ifndef __arm__
#include <iostream>

#endif
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
    id_t last_evaluated_child;

    ptr get_child_for_eval(id_t id)
    {
        last_evaluated_child = id;
        if(id >= children.size())
        {
            return nullptr;
        }
        return children[id];
    }

    id_t get_last_evaluated_child_id()
    {
        if(last_evaluated_child >= children.size())
        {
            last_evaluated_child = 0;
        }
        return last_evaluated_child;
    }

    virtual BehaviorState internal_evaluate(id_t id_child_for_evaluation) = 0;

    BehaviorState internal_evaluate()
    {
        return internal_evaluate(0);
    }
};



