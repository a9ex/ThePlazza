/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Input.hpp
*/

#pragma once

#include "Plazza.hpp"
#include "Pizza.hpp"
#include "PizzaBuilder.hpp"
#include "PizzaOrderParser.hpp"

namespace plazza {
    class Input {
        public:
            /**
            * @brief Handle user input via cin
            *
            */
            static void handleUserInput(
                plazza::Holders &holders,
                std::map<std::string, plazza::Pizza> &pizzas,
                std::vector<std::shared_ptr<plazza::Kitchen>> &kitchens,
                std::mutex &mutex
            );
    };
}
