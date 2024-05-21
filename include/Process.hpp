/*
** EPITECH PROJECT, 2024
** Process.hpp
** File description:
** Process.hpp
*/

#pragma once

#include <functional>
#include <unistd.h>

namespace process {
    class ForkProcess {
    public:
        ForkProcess(std::function<void()> child_func = []() {});
        ~ForkProcess() = default;

        bool isAlive() const;

        pid_t getPid() const { return this->_pid; }
        bool isChild() const { return this->_pid == 0; }
    private:
        pid_t _pid;
    };
}
