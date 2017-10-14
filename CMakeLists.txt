cmake_minimum_required(VERSION 3.9)
project(behavior_tree_simpler_way)

set(CMAKE_CXX_STANDARD 14)
# for normal usage:
#set(CMAKE_CXX_FLAGS "-O0 -g -Wpedantic -Wall -Weffc++ -Wextra")
# for dr. memory usage:
set(CMAKE_CXX_FLAGS "-O0 -ggdb -static-libgcc -static-libstdc++ -Wpedantic -Wall -Weffc++ -Wextra")
set(SOURCE_FILES behavior_system/BehaviorEmpty.cpp behavior_system/BehaviorEmpty.hpp behavior_system/IBehavior.cpp behavior_system/IBehavior.hpp behavior_system/primitive/BehaviorAction.cpp behavior_system/primitive/BehaviorAction.hpp behavior_system/primitive/BehaviorCondition.cpp behavior_system/primitive/BehaviorCondition.hpp behavior_system/composite/BehaviorSequence.cpp behavior_system/composite/BehaviorSequence.hpp)

add_executable(behavior_tree_simpler_way main.cpp ${SOURCE_FILES})
add_executable(unit_tests unit_tests/main_tests.cpp ${SOURCE_FILES} unit_tests/node_tests.cpp unit_tests/action_tests.cpp unit_tests/condition_tests.cpp unit_tests/sequence_tests.cpp)