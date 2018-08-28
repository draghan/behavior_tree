/*
    This file is distributed under MIT License.

    Copyright (c) 2018 draghan

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

//
// Created by draghan on 2017-10-17.
//

#include "../external/catch/single_include/catch2/catch.hpp"
#include "../behavior_system/tree/BehaviorTree.hpp"
#include "../behavior_system/primitive/BehaviorAction.hpp"
#include "../behavior_system/composite/BehaviorSequence.hpp"

TEST_CASE("Testing tree interface of BehaviorTree class", "[Tree]")
{
    SECTION("Test if \'running\' evaluation is correctly done")
    {
        int a1 = 0;
        int a2 = 0;
        int a3 = 0;
        auto action1 = [&]()
        {
            ++a1;
            return BehaviorState::success;
        };
        auto action2 = [&]()
        {
            ++a2;
            if(a2 < 3)
            {
                return BehaviorState::running;
            }
            else
            {
                return BehaviorState::success;
            }
        };
        auto action3 = [&]()
        {
            ++a3;
            return BehaviorState::success;
        };
        auto action_false = [&]()
        {
            return BehaviorState::failure;
        };
        BehaviorTree tree;
        tree.add_selector();
        tree.set_at_absolutely();
        tree.add_action(action_false);
        tree.add_sequence();
        tree.set_at_absolutely(1);
        tree.add_action(action1);
        tree.add_action(action2);
        tree.add_action(action3);
        tree.set_at_absolutely();
        REQUIRE(tree.get_node_count() == 6);

        REQUIRE(tree.evaluate() == BehaviorState::running);
        REQUIRE(a1 == 1);
        REQUIRE(a2 == 1);
        REQUIRE(a3 == 0);

        REQUIRE(tree.evaluate() == BehaviorState::running);
        REQUIRE(a1 == 1);
        REQUIRE(a2 == 2);
        REQUIRE(a3 == 0);

        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(a1 == 1);
        REQUIRE(a2 == 3);
        REQUIRE(a3 == 1);

        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(a1 == 2);
        REQUIRE(a2 == 4);
        REQUIRE(a3 == 2);
    }

    SECTION("Test parent-related methods of nodes")
    {
        /*
                 selector-root-0
              __________|____________
              |         |           |
            sequence-1  action-4  action-5
              ___|_______
              |         |
            action-2  action-3

         */

        BehaviorTree tree;
        tree.add_selector();
        tree.add_sequence();
        tree.set_at_relatively(0);
        tree.add_condition([]()
                           {
                               return true;
                           });
        tree.add_condition([]()
                           {
                               return true;
                           });
        tree.set_at_absolutely();
        tree.add_condition([]()
                           {
                               return true;
                           });
        tree.add_condition([]()
                           {
                               return true;
                           });

        REQUIRE(tree.get_node_count() == 6);
        auto root = tree.get();
        REQUIRE(root->get_id() == 0);

        tree.set_at_absolutely(0);
        auto seq_1 = tree.get();
        REQUIRE(seq_1->get_id() == 1);

        tree.set_at_absolutely(1);
        auto act_4 = tree.get();
        REQUIRE(act_4->get_id() == 4);

        tree.set_at_absolutely(2);
        auto act_5 = tree.get();
        REQUIRE(act_5->get_id() == 5);

        tree.set_at_absolutely(0, 0);
        auto act_2 = tree.get();
        REQUIRE(act_2->get_id() == 2);

        tree.set_at_absolutely(0, 1);
        auto act_3 = tree.get();
        REQUIRE(act_3->get_id() == 3);


        REQUIRE(root->get_parent() == nullptr);
        REQUIRE(seq_1->get_parent() == root);
        REQUIRE(act_4->get_parent() == root);
        REQUIRE(act_5->get_parent() == root);
        REQUIRE(act_2->get_parent() == seq_1);
        REQUIRE(act_3->get_parent() == seq_1);
    }


    SECTION("Constructor with root data")
    {
        BehaviorTree tree{new BehaviorAction{12, []()
        {
            return BehaviorState::running;
        }}};

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
        tree.add_action([&]()
                        {
                            ++counter;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++counter;
                            return BehaviorState::success;
                        });

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
        tree_0.add_action([&]()
                          {
                              ++counter_1;
                              return BehaviorState::success;
                          });
        tree_0.add_action([&]()
                          {
                              ++counter_2;
                              return BehaviorState::success;
                          });

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

//    SECTION("Testing operator+=") // todo
//    {
//        int counter_1 = 0;
//        int counter_2 = 0;
//        BehaviorTree tree1;
//        tree1.add_selector();
//        tree1.add_action([&]()
//                         {
//                             ++counter_1;
//                             return BehaviorState::failure;
//                         });
//        tree1.add_action([&]()
//                         {
//                             ++counter_1;
//                             return BehaviorState::failure;
//                         });
//
//        BehaviorTree tree2;
//        tree2.add_sequence();
//        tree2.add_action([&]()
//                         {
//                             ++counter_2;
//                             return BehaviorState ::success;
//                         });
//        tree2.add_action([&]()
//                         {
//                             ++counter_2;
//                             return BehaviorState::success;
//                         });
//        REQUIRE(tree1.evaluate() == BehaviorState::failure);
//        REQUIRE(counter_1 == 2);
//        REQUIRE(tree2.evaluate() == BehaviorState::success);
//        REQUIRE(counter_2 == 2);
//        counter_1 = counter_2 = 0;
//
//        tree1 += std::move(tree2);
//        REQUIRE(tree1.get_node_count() == 6);
//        REQUIRE(tree2.get_node_count() == 0);
//
//        REQUIRE(tree1.evaluate() == BehaviorState::success);
//        REQUIRE(counter_1 == 2);
//        REQUIRE(counter_2 == 2);
//        REQUIRE(tree2.evaluate() == BehaviorState::undefined);
//        REQUIRE(counter_1 == 2);
//        REQUIRE(counter_2 == 2);
//    }

    SECTION("Testing \'set_at_absolutely\' with {0-> {1-> {3(), 4()}} {2()}}")
    {
        int id = 0;
        int counter = 0;
        BehaviorTree tree{new BehaviorSequence(0)};
        tree.add_sequence();
        tree.add_action([&]()
                        {
                            ++counter;
                            id = 2;
                            return BehaviorState::undefined;
                        });

        REQUIRE(tree.set_at_absolutely(0) == true);
        tree.add_action([&]()
                        {
                            ++counter;
                            id = 3;
                            return BehaviorState::undefined;
                        });
        tree.add_action([&]()
                        {
                            ++counter;
                            id = 4;
                            return BehaviorState::undefined;
                        });

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
        tree.add_sequence();
        tree.add_action([&]()
                        {
                            ++counter;
                            id = 2;
                            return BehaviorState::undefined;
                        });

        REQUIRE(tree.set_at_absolutely(0) == true);
        tree.add_action([&]()
                        {
                            ++counter;
                            id = 3;
                            return BehaviorState::undefined;
                        });
        tree.add_action([&]()
                        {
                            ++counter;
                            id = 4;
                            return BehaviorState::undefined;
                        });

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
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });

        REQUIRE(tree.get_node_count() == 5);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 4);
        action_count = 0;

        tree.add_sequence();
        REQUIRE(tree.set_at_relatively(4) == true);
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });

        REQUIRE(tree.get_node_count() == 8);
        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 6);
    }

    SECTION("Tree with sequences of success and failure actions")
    {
        int action_count = 0;
        BehaviorTree tree{new BehaviorSequence(0)};
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });

        REQUIRE(tree.get_node_count() == 5);
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_count == 4);
        action_count = 0;

        tree.add_sequence();
        REQUIRE(tree.set_at_relatively(4) == true);
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::failure;
                        });
        tree.add_action([&]()
                        {
                            ++action_count;
                            return BehaviorState::success;
                        });

        REQUIRE(tree.get_node_count() == 8);
        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::failure);
        REQUIRE(action_count == 5);
    }
}

TEST_CASE("Testing factor part of BehaviorTree class", "[Tree]")
{
    SECTION("Adding one copy of each behavior node")
    {
        BehaviorTree tree;
        REQUIRE(tree.set_at_absolutely() == false);
        REQUIRE(tree.get_id() == BehaviorTree::undefined_id);
        REQUIRE(tree.get_node_count() == 0);
        REQUIRE(tree.get() == nullptr);

        tree.add_selector();
        REQUIRE(tree.get_id() == 0);
        REQUIRE(tree.get_node_count() == 1);
        REQUIRE(dynamic_cast<BehaviorSelector *>(tree.get()) != nullptr);

        tree.add_sequence();
        REQUIRE(tree.get_id() == 0);
        REQUIRE(tree.get_node_count() == 2);
        REQUIRE(dynamic_cast<BehaviorSelector *>(tree.get()) != nullptr);
        REQUIRE(tree.set_at_relatively(0) == true);
        REQUIRE(tree.get_id() == 1);
        REQUIRE(dynamic_cast<BehaviorSequence *>(tree.get()) != nullptr);
        REQUIRE(tree.set_at_absolutely(1) == false);

        REQUIRE(tree.set_at_absolutely(0) == true);
        REQUIRE(tree.add_condition([]()
                                   {
                                       return true;
                                   }));
        int action_counter = 0;
        REQUIRE(tree.add_action([&]()
                                {
                                    ++action_counter;
                                    return BehaviorState::success;
                                }));
        REQUIRE(tree.get_node_count() == 4);

        REQUIRE(tree.set_at_absolutely(0, 0));
        REQUIRE(tree.get_id() == 2);
        REQUIRE(dynamic_cast<BehaviorCondition *>(tree.get()) != nullptr);

        REQUIRE(tree.set_at_absolutely(0, 1));
        REQUIRE(tree.get_id() == 3);
        REQUIRE(dynamic_cast<BehaviorAction *>(tree.get()) != nullptr);

        REQUIRE(tree.set_at_absolutely(0, 2) == false);

        REQUIRE(tree.set_at_absolutely());
        REQUIRE(tree.evaluate() == BehaviorState::success);
        REQUIRE(action_counter == 1);
    }
}