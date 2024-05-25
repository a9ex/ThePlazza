/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** TestsIngredientStock.cpp
*/

#include <iostream>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include "IngredientStock.hpp"

Test(IngredientStock, test_impl)
{
    IngredientStock i;
    std::size_t j = 0;

    std::cout << "IngredientStock::IngredientStock()" << std::endl;
    for (; plazza::PizzaIngredient::INGREDIENT_ITER_END > j; ++j)
        cr_assert(eq(int, 5, i.getIngredient(static_cast<plazza::PizzaIngredient>(j))));
    std::cout << "IngredientStock::refillAll()" << std::endl;
    i.refillAll();
    for (j = 0; plazza::PizzaIngredient::INGREDIENT_ITER_END > j; ++j)
        cr_assert(eq(int, 6, i.getIngredient(static_cast<plazza::PizzaIngredient>(j))));
    std::cout << "IngredientStock::consume()" << std::endl;
    for (j = 0; plazza::PizzaIngredient::INGREDIENT_ITER_END > j; ++j)
        i.consume(static_cast<plazza::PizzaIngredient>(j));
    for (j = 0; plazza::PizzaIngredient::INGREDIENT_ITER_END > j; ++j)
        cr_assert(eq(int, 5, i.getIngredient(static_cast<plazza::PizzaIngredient>(j))));
    std::cout << "IngredientStock::~IngredientStock()" << std::endl;
}
