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

        private:
            /**
             * @brief Get the oven states of a kitchen
             *
             * @param kitchen the kitchen to get the oven states from
             * @return std::string the oven states
             */
            static std::string _getOvenStates(std::shared_ptr<plazza::Kitchen> kitchen);
            /**
             * @brief Get the ingredient stock of a kitchen
             *
             * @param spec the kitchen spec to get the ingredient stock from
             * @return std::string the ingredient stock
             */
            static std::string _getIngredientStock(plazza::KitchenSpec &spec);
    };
}
