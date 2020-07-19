# BarePi [ Unfinished / WIP ]

## Project Status
* Jul 2020: The Project began during the Covid-19 outbreak which gave me a bit more time for private projects.
Additionally I learned that I don't know enough about the ARM architecture when it comes to higher-end architectures like multicore on the Raspberry PI. I need a lot more research to understand how ARM deals with multiple cores, caching and exception states.
So the project is on hold until I have the time to research the mentioned topics a bit more.

## Goal
The goal of this project is to create a SDK for making Raspberry Applications without any operating system. The project is **currently not in a working condition**. 

The SDK should allow programmers to make fast real time applications that don't require the installation of a full operating system, and without having to worry too much about the boot sequence, differences between processors and hardware initialization.

It should also be made compatible with C++ even though the SDK itself will be written mostly in C and Assembly.

For debugging capability the Developer should be able to choose between the raspberries onboard serial interface and qemu debugging. The exact details and extra features are not fully worked out yet.