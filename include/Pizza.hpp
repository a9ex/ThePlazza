/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Pizza.hpp
*/

#pragma once

#include <string>
#include <vector>

namespace plazza {
    /**
     * @brief Represents the type of a pizza (do not modify the values)
     * Required by the subject
     */
    enum PizzaType {
        Regina = 1,
        Margarita = 2,
        Americana = 4,
        Fantasia = 8,
        TYPE_ITER_END,
    };

    /**
     * @brief Represents the size of a pizza (do not modify the values)
     *
     */
    enum PizzaSize {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16,
        SIZE_ITER_END,
    };

    /**
     * @brief Represents the ingredients of a pizza
     *
     */
    enum PizzaIngredient {
        Dough = 0,
        Tomato,
        Gruyere,
        Ham,
        Mushrooms,
        Steak,
        Eggplant,
        GoatCheese,
        ChiefLove,
        INGREDIENT_ITER_END,
    };

    // Represents a pizza
    class Pizza {
        public:
            Pizza() = default;
            ~Pizza() = default;

            /**
             * @brief Get the cooking time of the pizza
             *
             * @return double the cooking time in seconds
             */
            double getCookingTime() const;
            /**
             * @brief Get the Type of the pizza
             *
             * @return PizzaType the type of the pizza
             */
            PizzaType getType() const;
            /**
             * @brief Get the display name of the pizza
             *
             * @return std::string the name of the pizza
             */
            std::string getName() const;
            /**
             * @brief Get the Ingredients of the pizza
             *
             * @return std::vector<PizzaIngredient> a vector of ingredients
             */
            std::vector<PizzaIngredient> getIngredients() const;

            /**
             * @brief Set the cooking time of the pizza
             *
             * @param time the cooking time in seconds
             */
            void setCookingTime(double time);
            /**
             * @brief Set the type of the pizza (used internally)
             *
             * @param type the type of the pizza
             */
            void setType(PizzaType type);
            /**
             * @brief Set the display name of the pizza (in console)
             *
             * @param name the name of the pizza
             */
            void setName(const std::string &name);
            /**
             * @brief Set the ingredients of the pizza
             *
             * @param ingredients a vector of ingredients
             */
            void setIngredients(const std::vector<PizzaIngredient> &ingredients);
        protected:
        private:
            double _cookingTime;
            PizzaType _type;
            std::string _name;
            std::vector<PizzaIngredient> _ingredients;
    };
}
