/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** IngredientStock.cpp
*/

#include "IngredientStock.hpp"

IngredientStock::IngredientStock() : m_semaphore{1}
{
    std::size_t i = 0;

    for (; plazza::PizzaIngredient::IngredientCount > i; ++i)
        m_ingredients[i] = 5;
}

std::size_t IngredientStock::getIngredient(plazza::PizzaIngredient ingredient)
{
    std::size_t value = 0;

    m_semaphore.acquire();
    value = m_ingredients[ingredient];
    m_semaphore.release();
    return value;
}

void IngredientStock::refillAll()
{
    std::size_t i = 0;

    m_semaphore.acquire();
    for (; plazza::PizzaIngredient::IngredientCount > i; ++i)
        ++m_ingredients[i];
    m_semaphore.release();
}

void IngredientStock::consume(plazza::PizzaIngredient ingredient)
{
    m_semaphore.acquire();
    --m_ingredients[ingredient];
    m_semaphore.release();
}
