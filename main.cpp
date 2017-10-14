
#include <iostream>
#include "behavior_system/BehaviorEmpty.hpp"

#define REQUIRE if

int main()
{
    char character{'c'};
    std::string s{character};
    s += character;
    std::cout << s << std::endl;
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

    root->PrintPretty("", false, std::cout);

    for(auto& b : behaviors)
    {
        delete b;
    }
}

#undef REQUIRE