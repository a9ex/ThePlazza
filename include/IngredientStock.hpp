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

template <size_t I>
class IngredientStock {
    public:
        IngredientStock() : m_semaphore(I), m_ingredients(static_cast<size_t>(plazza::PizzaIngredient::INGREDIENT_ITER_END), 5) {}
        ~IngredientStock() = default;

        IngredientStock(const IngredientStock &other) : m_semaphore(I), m_ingredients(other.m_ingredients) {}
        IngredientStock& operator=(const IngredientStock&) {
            m_semaphore = I;
            m_ingredients = std::vector<std::size_t>(static_cast<size_t>(plazza::PizzaIngredient::INGREDIENT_ITER_END), 5);
            return *this;
        }

        std::size_t getIngredient(plazza::PizzaIngredient ingredient)
        {
            std::size_t value = 0;

            m_semaphore.acquire();
            value = m_ingredients[ingredient];
            m_semaphore.release();
            return value;
        }

        void refillAll()
        {
            m_semaphore.acquire();
            for (std::size_t &i : m_ingredients)
                ++i;
            m_semaphore.release();
        }

        void consume(plazza::PizzaIngredient ingredient)
        {
            m_semaphore.acquire();
            --m_ingredients[ingredient];
            m_semaphore.release();
        }

    private:
        std::counting_semaphore<I> m_semaphore;
        std::vector<std::size_t> m_ingredients;
};

#endif /* !__INGREDIENT_STOCK_HPP_ */
