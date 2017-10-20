//
// Created by draghan on 2017-10-14.
//

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

enum class BehaviorState :uint8_t
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

    auto get_id() const
    {
        return id;
    }

    auto set_id(uint32_t id)
    {
        this->id = id;
    }

    auto add_child(ptr child)
    {
        if (!can_have_children() || child == this)
        {
            return false;
        }

        children.push_back(child);
        return true;
    }

    auto get_number_of_children() const
    {
        return children.size();
    }

    ptr get_child(size_t index) const // todo: unify behavior of get_child and operator[] (or get rid of one of them)
    {
        if(index >= children.size())
        {
            return nullptr;
        }
        return children[index];
    }

    ptr get_last_child() const
    {
        if(children.empty())
        {
            return nullptr;
        }
        return *(children.end() - 1);
    }

    ptr get_parent() const
    {
        return parent;
    }

    bool is_leaf() const
    {
        return children.size() == 0;
    }

    ptr get_previous_sibling() const
    {
        if (parent == nullptr)
        {
            return nullptr;
        }
        size_t id_found;
        size_t id_last = parent->children.size();
        auto &siblings = parent->children;
        for (id_found = 0; id_found < id_last; ++id_found)
        {
            if (siblings[id_found]->id == this->id)
            {
                break;
            }
        }
        if (id_found == id_last || id_found == 0)
        {
            return nullptr;
        } else
        {
            return siblings[id_found - 1];
        }
    }

    ptr get_next_sibling() const
    {
        if (parent == nullptr)
        {
            return nullptr;
        }
        size_t id_found;
        size_t id_last = parent->children.size();
        auto &siblings = parent->children;
        for (id_found = 0; id_found < id_last; ++id_found)
        {
            if (siblings[id_found]->id == this->id)
            {
                break;
            }
        }
        if (id_found == id_last || id_found == id_last - 1)
        {
            return nullptr;
        } else
        {
            return siblings[id_found + 1];
        }
    }

    ptr get_left_most_sibling() const
    {
        if (parent == nullptr)
        {
            return nullptr;
        }
        auto &siblings = parent->children;
        if (siblings.size() < 2)
        {
            return nullptr;
        } else
        {
            return siblings[0];
        }
    }

    ptr get_left_most_child() const
    {
        if (children.size() == 0)
        {
            return nullptr;
        } else
        {
            return children[0];
        }
    }

    ptr get_right_most_child() const
    {
        if (children.size() == 0)
        {
            return nullptr;
        } else
        {
            return children[children.size() - 1];
        }
    }


    void PrintPretty(std::string indent, bool last, std::ostream& stream) // todo: clean print functions
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

    explicit IBehavior(ptr parent, uint32_t id = 0)
            : children{}, parent{parent}, id{id}, status{BehaviorState::undefined}
    {}

    virtual ~IBehavior() = default;

    BehaviorState evaluate()
    {
        status = internal_evaluate();
        return status;
    }

    BehaviorState get_status() const
    {
        return status;
    }

    bool operator==(const IBehavior&)
    {
        return false;
    }

    virtual void print(std::ostream& stream)
    {
        stream << "IBehavior [" << id << "]\n";
    }

    virtual bool can_have_children() = 0;
protected:
    std::vector<ptr> children;
    ptr parent;
    uint32_t id;
    BehaviorState status;

    virtual BehaviorState internal_evaluate() = 0;
};



