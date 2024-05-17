/*
** EPITECH PROJECT, 2024
** Pipe.hpp
** File description:
** Pipe.hpp
*/

#pragma once

#include <string>
#include <vector>

namespace pipe {
    class Pipe {
    public:
        Pipe(std::string path);
        ~Pipe();

        void write(std::vector<char> buffer);
        std::vector<char> read();
    private:
    };
}
