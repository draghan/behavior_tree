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
        if(child == this)
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

    ptr get_last_child() const // todo: unify behavior of get_child and operator[] (or get rid of one of them)
    {
        if(children.empty())
        {
            return nullptr;
        }
        return *(children.end() - 1);
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

    explicit IBehavior(uint32_t id = 0)
            : children{}, id{id}, status{BehaviorState::undefined}
    {}

    virtual ~IBehavior()
    {
        std::cout << "Destructor of IBehavior [" << id << "]\n";
    }

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

protected:
    std::vector<ptr> children;
    uint32_t id;
    BehaviorState status;


    virtual BehaviorState internal_evaluate() = 0;
};



