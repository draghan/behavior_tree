## What's this project?

This is C++ implementation of behavior trees AI system. It has no dependencies, except unit tests part: I've used [Catch library](https://github.com/catchorg/Catch2) (its only header is located on 'external' directory). This project is wrote in C++14 standard.

## How does it work?

It defines basic types of nodes used in behavior trees, like composites (selector and sequence), decorators (link, inversion, loop) and primitives (action and condition).
To provide resource-safe interface project provides BehaviorTree class, which is kind of manager for raw behavior nodes.
In the beginning you have to create instance of BehaviorTree class and full it with behavior nodes. Then you can call evaluate() method on it and check status, probably in some periodic maneer. 

Code of the behavior trees system is located in 'behavior_system' directory.
There is also 'external' directory, which includes [Catch](https://github.com/catchorg/Catch2) library header.
Last part of repository is 'unit_tests' directory which contains unit tests of the behavior system, based on [Catch](https://github.com/catchorg/Catch2).

## Who will use this repo or project?

Everyone who needs behavior trees based AI system. :)

## What is the purpose of this project?

This project was designed as a part of my BSc thesis - "The control system of an autonomous mobile robot based on behavioral tree". The robot runs on ARM Cortex processor so in the code you can find parts designed for ARM architecture, but whole system is fully portable - source code is completely compatible with ISO C++14 standard. The main goal in the implementation was portability - you can use the system in embedded environment or in PC game for example.