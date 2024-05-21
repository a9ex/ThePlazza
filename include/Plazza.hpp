/*
** EPITECH PROJECT, 2024
** Plazza.hpp
** File description:
** Plazza.hpp
*/

#pragma once

#include "File.hpp"
#include "Process.hpp"
#include <string>
#include <optional>

namespace plazza {
    class Kitchen {
    public :
        Kitchen(std::string id);
        void cookPizza(std::string pizza);
        void closeKitchen();

    private:
        std::string _id;
        std::optional<process::ForkProcess> _process;
        std::optional<file::Pipe> _output_pipe;
        std::optional<file::Pipe> _input_pipe;
    };

    class LocalKitchen {
    public:
        LocalKitchen(std::string id);
    private:
        std::string _id;
        file::Pipe _input_pipe;
        file::Pipe _output_pipe;
    };
}
