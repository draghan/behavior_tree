//
// Created by draghan on 2017-10-14.
//

#include "../external/catch.hpp"
#include "../behavior_system/BehaviorEmpty.hpp"


TEST_CASE("Testing typical node of tree interface - BehaviorEmpty class", "[BehaviorEmpty]")
{
    SECTION("Testing default behavior of \'empty\' BehaviorEmpty object")
    {
        BehaviorEmpty node{5};
        REQUIRE(node.get_id() == 5);
        REQUIRE(node.get_number_of_children() == 0);
        REQUIRE(node.get_child(5) == nullptr);
        REQUIRE(node.get_child(-5) == nullptr);
        REQUIRE(node.get_child(0) == nullptr);
        REQUIRE(node.get_last_child() == nullptr);

        REQUIRE_NOTHROW(node.get_child(5));
        REQUIRE_NOTHROW(node.get_child(-5));
        REQUIRE_NOTHROW(node.get_child(0));
        REQUIRE_NOTHROW(node.get_last_child());
    }

    SECTION("Testing behavior of one-generation Nodes", "[BehaviorEmpty]")
    {
        BehaviorEmpty root{0};
        BehaviorEmpty child_1{1};
        BehaviorEmpty child_2{2};

        REQUIRE(root.add_child(&child_1) == true);
        REQUIRE(root.add_child(&child_2) == true);

        REQUIRE(child_1.get_id() == 1);
        REQUIRE(child_2.get_id() == 2);

        REQUIRE(child_1.get_child(0) == nullptr);
        REQUIRE(child_2.get_child(0) == nullptr);

        REQUIRE(root.get_id() == 0);
        REQUIRE(root.get_number_of_children() == 2);
        REQUIRE(root.get_child(0) == &child_1);
        REQUIRE(root.get_child(1) == &child_2);
        REQUIRE(root.get_child(2) == nullptr);
    }

    SECTION("Test managing with std::vector<IBehavior::ptr>")
    {
        std::vector<IBehavior::ptr> behaviors;
        behaviors.push_back(new BehaviorEmpty{11});
        behaviors.push_back(new BehaviorEmpty{12});
        behaviors.push_back(new BehaviorEmpty{13});

        REQUIRE(behaviors.size() == 3);
        REQUIRE(behaviors[0]->get_id() == 11);
        REQUIRE(behaviors[1]->get_id() == 12);
        REQUIRE(behaviors[2]->get_id() == 13);

        REQUIRE(behaviors[0]->get_child(0) == nullptr);
        REQUIRE(behaviors[1]->get_child(0) == nullptr);
        REQUIRE(behaviors[2]->get_child(0) == nullptr);

        auto root = behaviors[0];
        REQUIRE(root->add_child(behaviors[1]) == true);
        REQUIRE(root->add_child(behaviors[2]) == true);
        REQUIRE(root->add_child(behaviors[0]) == false);


        REQUIRE(root->get_id() == 11);
        REQUIRE(root->get_number_of_children() == 2);

        IBehavior::ptr child_0 = root->get_child(0);
        IBehavior::ptr child_1 = root->get_child(1);

        REQUIRE(child_0->get_id() == 12);
        REQUIRE(child_1->get_id() == 13);

        REQUIRE(child_0->get_number_of_children() == 0);
        REQUIRE(child_1->get_number_of_children() == 0);

        REQUIRE(child_0->get_child(0) == nullptr);
        REQUIRE(child_1->get_child(0) == nullptr);

        REQUIRE(child_0->evaluate() == BehaviorState::undefined);
        REQUIRE(child_1->evaluate() == BehaviorState::undefined);

        REQUIRE(child_0->add_child(behaviors[2]) == true);
        REQUIRE(child_0->get_number_of_children() == 1);
        REQUIRE(child_0->get_child(0) == child_1);
        REQUIRE(root->get_number_of_children() == 2);

        REQUIRE(behaviors.size() == 3);

        for(auto& b : behaviors)
        {
            delete b;
        }
    }
}