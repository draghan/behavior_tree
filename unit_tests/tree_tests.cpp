//
// Created by draghan on 2017-10-17.
//

#include "../external/catch.hpp"
#include "../behavior_system/tree/BehaviorTree.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"
#include "../behavior_system/composite/BehaviorSequence.hpp"

TEST_CASE("Testing tree interface of BehaviorTree class", "[Tree]")
{
    SECTION("Constructor with root data")
    {
        BehaviorTree tree{new BehaviorAction{12, []() { return BehaviorState::running; }}};

        REQUIRE(tree.get_node_count() == 1);

        REQUIRE(tree.set_at_id(0) == true);
        auto root = tree.get();
        REQUIRE(root->get_number_of_children() == 0);
        REQUIRE(root->get_id() == 0);
        REQUIRE(root->evaluate() == BehaviorState::running);

        REQUIRE(tree.set_at_id(1) == false);
        REQUIRE(tree.set_at_id(-1) == false);

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(tree.get() == root);

        REQUIRE(tree.set_at_absolutely(0) == false);

        REQUIRE(tree.set_at_relatively() == true);
        REQUIRE(tree.get() == root);
    }

    SECTION("Test correct behavior after adding 2 children to root")
    {
        int counter = 0;
        BehaviorTree tree{new BehaviorSequence(10)};
        tree.add_child(new BehaviorAction(10, [&]() {
            ++counter;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(10, [&]() {
            ++counter;
            return BehaviorState::success;
        }));

        REQUIRE(tree.get_node_count() == 3);

        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(counter == 2);

        REQUIRE(tree.set_at_absolutely(0) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);

        counter = 0;
        REQUIRE(tree.set_at_absolutely(1) == true);
        REQUIRE(tree.get_id() == 2);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(counter == 1);

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(tree.get_id() == 1);

        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_relatively(-1) == false);
        REQUIRE(tree.set_at_relatively(1) == false);

    }

    SECTION("Test move constructor")
    {
        int counter_1 = 0;
        int counter_2 = 0;
        BehaviorTree tree_0{new BehaviorSequence(10)};
        tree_0.add_child(new BehaviorAction(10, [&]() {
            ++counter_1;
            return BehaviorState::success;
        }));
        tree_0.add_child(new BehaviorAction(10, [&]() {
            ++counter_2;
            return BehaviorState::success;
        }));

        REQUIRE(tree_0.get_node_count() == 3);

        BehaviorTree tree_1{std::move(tree_0)};
        REQUIRE(tree_0.get_node_count() == 0);
        REQUIRE(tree_1.get_node_count() == 3);

        REQUIRE(tree_0.set_at_absolutely() == false);
        REQUIRE(tree_0.get() == nullptr);
        REQUIRE(tree_1.set_at_absolutely() == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree_1.get()) != nullptr);
        REQUIRE(tree_1.set_at_relatively(0) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree_1.get()) != nullptr);
        REQUIRE(tree_1.set_at_absolutely() == true);
        REQUIRE(tree_1.get_id() == 0);
        REQUIRE(tree_1.set_at_absolutely(0) == true);
        REQUIRE(tree_1.get_id() == 1);
        REQUIRE(tree_1.set_at_absolutely(1) == true);
        REQUIRE(tree_1.get_id() == 2);
        REQUIRE(tree_1.set_at_absolutely(2) == false);

        REQUIRE(tree_0.evaluate() == BehaviorState::undefined);
        REQUIRE(tree_0.get_id() == BehaviorTree::undefined_id);
        REQUIRE(tree_0.set_at_relatively(0) == false);
        REQUIRE(tree_0.set_at_absolutely(0) == false);
        REQUIRE(tree_0.set_at_absolutely(0, 1, 2) == false);
        REQUIRE(tree_0.set_at_relatively(0, 1, 2) == false);

        REQUIRE(tree_1.set_at_absolutely());
        REQUIRE(tree_1.evaluate() == BehaviorState::success);
        REQUIRE(counter_1 == 1);
        REQUIRE(counter_2 == 1);
    }

    SECTION("Testing \'set_at_absolutely\' with {0-> {1-> {3(), 4()}} {2()}}")
    {
        int id = 0;
        int counter = 0;
        BehaviorTree tree{new BehaviorSequence(0)};
        tree.add_child(new BehaviorSequence(0));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++counter;
            id = 2;
            return BehaviorState::undefined;
        }));

        REQUIRE(tree.set_at_absolutely(0) == true);
        tree.add_child(new BehaviorAction(0, [&]() {
            ++counter;
            id = 3;
            return BehaviorState::undefined;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++counter;
            id = 4;
            return BehaviorState::undefined;
        }));

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 0);

        REQUIRE(tree.set_at_absolutely(0) == true);
        REQUIRE(tree.set_at_absolutely(1) == true);
        REQUIRE(tree.set_at_absolutely(1, 0) == false);
        REQUIRE(tree.set_at_absolutely(2) == false);
        REQUIRE(tree.set_at_absolutely(0, 0) == true);
        REQUIRE(tree.set_at_absolutely(0, 1) == true);
        REQUIRE(tree.set_at_absolutely(0, 2) == false);
        REQUIRE(tree.set_at_absolutely(-1) == false);
        REQUIRE(tree.set_at_absolutely(100) == false);

        REQUIRE(tree.set_at_absolutely(0, 0));
        REQUIRE(tree.evaluate() == BehaviorState::undefined);
        REQUIRE(counter == 1);
        REQUIRE(id == 3);
        REQUIRE(tree.get_id() == 3);
        counter = 0;
        REQUIRE(tree.set_at_absolutely(0, 1));
        REQUIRE(tree.evaluate() == BehaviorState::undefined);
        REQUIRE(counter == 1);
        REQUIRE(id == 4);
        REQUIRE(tree.get_id() == 4);

        counter = 0;
        REQUIRE(tree.set_at_absolutely(1));
        REQUIRE(tree.evaluate() == BehaviorState::undefined);
        REQUIRE(counter == 1);
        REQUIRE(id == 2);
        REQUIRE(tree.get_id() == 2);
    }

    SECTION("Testing \'set_at_relatively\' with {0-> {1-> {3(), 4()}} {2()}}")
    {
        int id = 0;
        int counter = 0;
        BehaviorTree tree{new BehaviorSequence(0)};
        tree.add_child(new BehaviorSequence(0));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++counter;
            id = 2;
            return BehaviorState::undefined;
        }));

        REQUIRE(tree.set_at_absolutely(0) == true);
        tree.add_child(new BehaviorAction(0, [&]() {
            ++counter;
            id = 3;
            return BehaviorState::undefined;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++counter;
            id = 4;
            return BehaviorState::undefined;
        }));

        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 0);

        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 1);

        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 3);
        REQUIRE(tree.evaluate() == BehaviorState::undefined);
        REQUIRE(counter == 1);
        REQUIRE(id == 3);

        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_absolutely() == true);
        REQUIRE(tree.set_at_relatively(0, 1) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 4);
        counter = 0;
        REQUIRE(tree.evaluate() == BehaviorState::undefined);
        REQUIRE(counter == 1);
        REQUIRE(id == 4);
        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_absolutely() == true);

        REQUIRE(tree.set_at_relatively(1) == true);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);
        REQUIRE(tree.get_id() == 2);
        counter = 0;
        REQUIRE(tree.evaluate() == BehaviorState::undefined);
        REQUIRE(counter == 1);
        REQUIRE(id == 2);
        REQUIRE(tree.set_at_relatively(0) == false);
        REQUIRE(tree.set_at_relatively(1) == false);
        REQUIRE(tree.set_at_relatively(-1) == false);
    }
}

TEST_CASE("Testing \'behavior part\' interface of BehaviorTree class", "[Tree]")
{
    SECTION("Tree with sequences of success actions")
    {
        int action_count = 0;
        BehaviorTree tree{new BehaviorSequence(0)};
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));

        REQUIRE(tree.get_node_count() == 5);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 4);
        action_count = 0;

        tree.add_child(new BehaviorSequence(0));
        REQUIRE(tree.set_at_relatively(4) == true);
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));

        REQUIRE(tree.get_node_count() == 8);
        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 6);
    }

    SECTION("Tree with sequences of success and failure actions")
    {
        int action_count = 0;
        BehaviorTree tree{new BehaviorSequence(0)};
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));

        REQUIRE(tree.get_node_count() == 5);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 4);
        action_count = 0;

        tree.add_child(new BehaviorSequence(0));
        REQUIRE(tree.set_at_relatively(4) == true);
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::failure;
        }));
        tree.add_child(new BehaviorAction(0, [&]() {
            ++action_count;
            return BehaviorState::success;
        }));

        REQUIRE(tree.get_node_count() == 8);
        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::failure);
        REQUIRE(action_count == 5);
    }
}
