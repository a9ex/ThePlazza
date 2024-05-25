/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** PizzaBuilder.hpp
*/

#include "Pizza.hpp"
#include <map>
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
            PizzaBuilder &setPizzaType(PizzaType type);
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
            PizzaBuilder &setIngredients(const std::vector<PizzaIngredient> &ingredients);

            /**
             * @brief Build the pizza
             *
             * @return Pizza the pizza
             */
            Pizza build();

            /**
             * @brief Adds the pizza to a map of pizzas
             *
             * @param map the map of pizzas
             */
            PizzaBuilder &addToMap(std::map<std::string, plazza::Pizza> &pizzas);
        protected:
        private:
            Pizza _pizza;
    };
}
