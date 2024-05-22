/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Main.cpp
*/

#include <iostream>
#include "Process.hpp"
#include "File.hpp"
#include "Plazza.hpp"
#include "ThreadPool.hpp"

#ifndef CRITERION

int main(void)
{
    std::size_t i = 0;
    ThreadPool pool(2);

    pool.run();
    for (; i < 1000; ++i)
        pool.addTask([i, &pool] {
            std::cout << "Task " << i << " started" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " done !" << std::endl;
            pool.addTask([] {
                std::cout << "Hello, World !" << std::endl;
            });
        });
    std::this_thread::sleep_for(std::chrono::seconds(5));
    pool.close(true);
    return 0;
}

#endif
