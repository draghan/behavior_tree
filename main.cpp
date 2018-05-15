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

#include <iostream>
#include "behavior_system/tree/BehaviorTree.hpp"


class Actor
{
public:

    Actor(): said_hello{false},
             rotating{false}
    {
    }

    void say_hello()
    {
        std::cout << "Hello!\n";
        said_hello = true;
    }

    bool can_look_around()
    {
        static int look_around_counter = 0;
        constexpr int look_around_limiter = 5;

        if(look_around_counter >= look_around_limiter)
        {
            return false;
        }

        ++look_around_counter;
        return true;
    }

    bool is_stopped()
    {
        static int stopped_counter = 0;
        ++stopped_counter;

        if(stopped_counter % 3) // 2/3 of checks will fail
        {
            return true;
        }
        return false;
    }

    void stop()
    {
    }

    bool is_still_rotating()
    {
        static int rotate_counter = 0;
        if(!rotating)
        {
            return false;
        }

        ++rotate_counter;

        if(rotate_counter % 5) // 4/5 of checks will confirm that actor is rotating
        {
            return false;
        }
        return true;
    }

    void rotate(int degrees)
    {
        if(rotating)
        {
            return;
        }
        rotating = true;
        std::cout << "I'll turn " << degrees << " degrees!\n";
    }

    bool said_hello;

private:
    bool rotating;
};

int main()
{
    Actor hero;

    BehaviorTree example_tree;
    example_tree.add_selector(); // root

    example_tree.add_sequence(); // sayHello
    example_tree.add_sequence(); // lookAround

    // set active node: first (zero-based indexing) child of root:
    example_tree.set_at_absolutely(0);

    example_tree.add_invert();

    // go to first child of 'sayHello' sequence node:
    example_tree.set_at_relatively(0);

    // saidHello condition node:
    example_tree.add_condition([&hero]()
                               {
                                   return hero.said_hello;
                               });

    // get back to the 'sayHello' sequence:
    example_tree.set_at_absolutely(0);

    // add say "Hello" action node:
    example_tree.add_action([&hero]()
                            {
                                hero.say_hello();
                                return BehaviorState::success;
                            });

    // set as active 'lookAround' sequence:
    example_tree.set_at_absolutely(1);

    // canLookAround condition node:
    example_tree.add_condition([&hero]()
                               {
                                   return hero.can_look_around();
                               });

    // Stop action node:
    example_tree.add_action([&hero]()
                            {
                                if(hero.is_stopped())
                                {
                                    return BehaviorState::success;
                                }
                                hero.stop();
                                return BehaviorState::running;
                            });

    // Rotate360 action:
    example_tree.add_action([&hero]()
                            {
                                hero.rotate(360);
                                if(hero.is_still_rotating())
                                {
                                    return BehaviorState::running;
                                }
                                return BehaviorState::success;
                            });

    // If we want to evaluate whole tree, we have to set root node as active:
    example_tree.set_at_absolutely();

    for(int i = 0; i < 15; ++i)
    {
        std::cout << i << " iteration\n";

        auto state = example_tree.evaluate();

        std::cout << "- returned state: ";
        switch(state)
        {
            case BehaviorState::failure:
                std::cout << "failure";
                break;
            case BehaviorState::running:
                std::cout << "running";
                break;
            case BehaviorState::success:
                std::cout << "success";
                break;
            case BehaviorState::undefined:
                std::cout << "undefined";
                break;
        }
        std::cout << "\n---\n";
    }
}