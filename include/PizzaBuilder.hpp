/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** PizzaBuilder.hpp
*/

#include "Pizza.hpp"
#include <string>
#include <vector>

#pragma once

namespace plazza {
    class PizzaBuilder {
        public:
            PizzaBuilder() = default;
            ~PizzaBuilder() = default;

            PizzaBuilder &setPizzaType(PizzaType type);
            PizzaBuilder &setPizzaName(const std::string &name);
            PizzaBuilder &setCookingTime(double time);
            PizzaBuilder &setIngredients(const std::vector<PizzaIngredient> &ingredients);

            Pizza build();
        protected:
        private:
            Pizza _pizza;
    };
}
