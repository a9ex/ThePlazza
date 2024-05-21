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
    // Represents a pizza builder, used to create easily a pizza
    class PizzaBuilder {
        public:
            PizzaBuilder() = default;
            ~PizzaBuilder() = default;

            /**
             * @brief Set the type of the pizza
             *
             * @param type the type of the pizza
             * @return PizzaBuilder& the builder (to chain calls)
             */
            PizzaBuilder &setPizzaType(Pizza::PizzaType type);
            /**
             * @brief Set the name of the pizza
             *
             * @param name the name of the pizza
             * @return PizzaBuilder& the builder (to chain calls)
             */
            PizzaBuilder &setPizzaName(const std::string &name);
            /**
             * @brief Set the cooking time of the pizza
             *
             * @param time the cooking time in seconds
             * @return PizzaBuilder& the builder (to chain calls)
             */
            PizzaBuilder &setCookingTime(double time);
            /**
             * @brief Set the ingredients of the pizza
             *
             * @param ingredients a vector of ingredients
             * @return PizzaBuilder& the builder (to chain calls)
             */
            PizzaBuilder &setIngredients(const std::vector<Pizza::PizzaIngredient> &ingredients);

            /**
             * @brief Build the pizza
             *
             * @return Pizza the pizza
             */
            Pizza build();
        protected:
        private:
            Pizza _pizza;
    };
}
