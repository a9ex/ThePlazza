/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Main.cpp
*/

#include <iostream>
#include <Process.hpp>

#ifndef CRITERION

int main(void)
{
    process::ForkProcess fp;

    if (fp.isChild()) {
        std::cout << "Hello from child!\n";
    } else {
        std::cout << "Hello from parent!\n";
    }

    return 0;
}

#endif
