//
// Created by draghan on 2017-10-14.
//

#include "IBehavior.hpp"

#ifndef __arm__
void IBehavior::PrintPretty(std::string indent, bool last, std::ostream &stream) // todo: clean print functions
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

void IBehavior::print(std::ostream &stream)
{
    stream << "IBehavior [" << id << "]\n";
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

IBehavior::ptr IBehavior::get_child(size_t index) const // todo: unify behavior of get_child and operator[] (or get rid of one of them)
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
        :
#ifndef __arm__
        draw_helper{this},
#endif
          children{},
          parent{parent},
          id{id},
          status{BehaviorState::undefined}
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
