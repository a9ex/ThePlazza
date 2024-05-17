/*
** EPITECH PROJECT, 2024
** Process.hpp
** File description:
** Process.hpp
*/

#pragma once

#include <unistd.h>

namespace process {
    class ForkProcess {
    public:
        ForkProcess();
        ~ForkProcess() { this->kill(); }

        void kill();
        bool isAlive() const;

        pid_t getPid() const { return this->_pid; }
    private:
        pid_t _pid;
    };
}
