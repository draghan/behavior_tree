//
// Created by draghan on 2017-10-21.
//

#include "IBehavior.hpp"

DrawHelper::DrawHelper(class IBehavior *owner)
        : x{0},
          y{0},
          mod{0.0f},
          width{0.0f},
          height{0.0f},
          owner{owner}
{}

bool DrawHelper::is_leaf() const
{
    return owner->children.size() == 0;
}

IBehavior *DrawHelper::get_previous_sibling() const
{
    if (owner->parent == nullptr)
    {
        return nullptr;
    }
    size_t id_found;
    size_t id_last = owner->parent->children.size();
    auto &siblings = owner->parent->children;
    for (id_found = 0; id_found < id_last; ++id_found)
    {
        if (siblings[id_found]->id == owner->id)
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

IBehavior *DrawHelper::get_next_sibling() const
{
    if (owner->parent == nullptr)
    {
        return nullptr;
    }
    size_t id_found;
    size_t id_last = owner->parent->children.size();
    auto &siblings = owner->parent->children;
    for (id_found = 0; id_found < id_last; ++id_found)
    {
        if (siblings[id_found]->id == owner->id)
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

IBehavior *DrawHelper::get_left_most_sibling() const
{
    if (owner->parent == nullptr)
    {
        return nullptr;
    }
    auto &siblings = owner->parent->children;
    if (siblings.size() < 2)
    {
        return nullptr;
    } else
    {
        return siblings[0];
    }
}

IBehavior *DrawHelper::get_left_most_child() const
{
    if (owner->children.size() == 0)
    {
        return nullptr;
    } else
    {
        return owner->children[0];
    }
}

IBehavior *DrawHelper::get_right_most_child() const
{
    if (owner->children.size() == 0)
    {
        return nullptr;
    } else
    {
        return owner->children[owner->children.size() - 1];
    }
}
