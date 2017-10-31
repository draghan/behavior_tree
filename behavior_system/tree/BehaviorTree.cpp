//
// Created by draghan on 2017-10-15.
//

#include "BehaviorTree.hpp"
#include "../Decorator/DecoratorLink.hpp"

const BehaviorTree::id_t BehaviorTree::undefined_id{std::numeric_limits<BehaviorTree::id_t>::max()};
const BehaviorTree::id_t BehaviorTree::id_any{0};

BehaviorTree::BehaviorTree(IBehavior::ptr root)
        : nodes{1, root},
          root{nodes[0]},
          active{root},
          last_id{0},
          operation_correct{true}
{
    root->set_id(last_id);
    ++last_id;
}

BehaviorTree::BehaviorTree()
        : nodes{},
          root{nullptr},
          active{nullptr},
          last_id{0},
          operation_correct{true}
{
}

BehaviorTree::BehaviorTree(BehaviorTree &&bt) noexcept
        : nodes{std::move(bt.nodes)},
          root{bt.root},
          active{bt.active},
          last_id{bt.last_id},
          operation_correct{bt.operation_correct}
{
    bt.root = nullptr;
    bt.active = nullptr;
    bt.last_id = 0;
}

BehaviorTree &BehaviorTree::operator=(BehaviorTree &&bt) noexcept
{
    delete_nodes();

    nodes = std::move(bt.nodes);
    root = bt.root;
    active = bt.active;
    last_id = bt.last_id;
    operation_correct = bt.operation_correct;

    bt.nodes.clear();
    bt.root = nullptr;
    bt.active = nullptr;
    return *this;
}

BehaviorTree &BehaviorTree::operator+=(BehaviorTree &&bt) noexcept // todo check if it is required to relocate vectors of children in moved nodes
{
    std::move(std::begin(bt.nodes), std::end(bt.nodes), std::back_inserter(nodes));
    bt.nodes.clear();
    bt.root = nullptr;
    bt.active = nullptr;

    for(size_t id = last_id; id < nodes.size(); ++id)
    {
        nodes[id]->set_id(id);
    }
    last_id = nodes.size();

    return *this;
}

BehaviorTree::~BehaviorTree()
{
    delete_nodes();
}


bool BehaviorTree::set_at_relatively()
{
    return active != nullptr;
}

bool BehaviorTree::set_at_absolutely()
{
    back_to_root();
    return active != nullptr;
}

bool BehaviorTree::set_at_id(BehaviorTree::id_t wanted_id)
{
    auto found = std::find_if(nodes.begin(), nodes.end(), [&](auto &node)
    {
        return node->get_id() == wanted_id;
    });

    if(found == nodes.end())
    {
        return false;
    }
    else
    {
        active = *found;
        return true;
    }
}

IBehavior::ptr BehaviorTree::get()
{
    return active;
}

BehaviorTree::id_t BehaviorTree::get_id() const
{
    if(active == nullptr)
    {
        return undefined_id;
    }
    return active->get_id();
}

BehaviorTree::id_t BehaviorTree::get_node_count() const
{
    return nodes.size();
}

bool BehaviorTree::add_child(IBehavior::ptr &&child)
{
#ifndef __arm__
    try
    {
#endif
        nodes.emplace_back(child);
#ifndef __arm__
    }
    catch(...)
    {
        return false;
    }
#endif
    if(nodes.size() == 1)
    {
        root = nodes[0];
        active = root;
        root->set_id(last_id);
        ++last_id;
        return true;
    }

    auto added_node = get_last_node();
    if(added_node == nullptr)
    {
        return false;
    }
    added_node->set_id(last_id);
    ++last_id;
    return active->add_child(added_node);
}

bool BehaviorTree::add_link(BehaviorTree *link)
{
    return add_child(new DecoratorLink(link, id_any, active));
}


bool BehaviorTree::add_sequence()
{
    return add_child(new BehaviorSequence(id_any, active));
}

bool BehaviorTree::add_selector()
{
    return add_child(new BehaviorSelector(id_any, active));
}

bool BehaviorTree::add_action(BehaviorAction::action_t &&action)
{
    return add_child(new BehaviorAction(id_any, std::move(action), active));
}

bool BehaviorTree::add_condition(BehaviorCondition::predicate_t &&predicate)
{
    return add_child(new BehaviorCondition(id_any, std::move(predicate), active));
}

#ifndef __arm__
void BehaviorTree::print(std::ostream &stream)
{
    bool root_is_lonely{root->get_number_of_children() == 0};
    root->PrintPretty("", root_is_lonely, stream);
}

#endif

BehaviorState BehaviorTree::evaluate()
{
    if(active == nullptr)
    {
        return BehaviorState::undefined;
    }
    else
    {
        return active->evaluate();
    }
}

IBehavior::ptr BehaviorTree::get_last_node()
{
    if(!nodes.empty())
    {
        return *(nodes.end() - 1);
    }
    else
    {
        return nullptr;
    }
}

void BehaviorTree::go_to_node_relatively(size_t x)
{
    if(active == nullptr)
    {
        operation_correct = false;
        return;
    }

    auto temp = active->get_child(x);

    if(temp == nullptr)
    {
        operation_correct = false;
    }
    else
    {
        active = temp;
    }
}

void BehaviorTree::back_to_root()
{
    active = root;
}

void BehaviorTree::delete_nodes()
{
    for(auto &node : nodes)
    {
        delete node;
    }
    nodes.clear();
}

void BehaviorTree::clear()
{
    delete_nodes();
    root = nullptr;
    active = nullptr;
    last_id = 0;
    operation_correct = true;
}
