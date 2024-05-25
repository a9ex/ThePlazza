/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** IngredientStock.hpp
*/

#ifndef __INGREDIENT_STOCK_HPP_
    #define __INGREDIENT_STOCK_HPP_
    #include <semaphore>
    #include "Pizza.hpp"

class IngredientStock {
    public:
        IngredientStock();
        IngredientStock(const IngredientStock &);
        std::size_t getIngredient(plazza::PizzaIngredient ingredient);
        void refillAll();
        void consume(plazza::PizzaIngredient ingredient);
    private:
        std::binary_semaphore m_semaphore;
        std::size_t m_ingredients[plazza::PizzaIngredient::INGREDIENT_ITER_END];
};

#endif /* !__INGREDIENT_STOCK_HPP_ */
