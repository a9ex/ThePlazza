/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Pizza.cpp
*/

#include "Pizza.hpp"

double plazza::Pizza::getCookingTime() const
{
    return _cookingTime;
}

plazza::Pizza::PizzaType plazza::Pizza::getType() const
{
    return _type;
}

std::string plazza::Pizza::getName() const
{
    return _name;
}

std::vector<plazza::Pizza::PizzaIngredient> plazza::Pizza::getIngredients() const
{
    return _ingredients;
}

void plazza::Pizza::setCookingTime(double time)
{
    _cookingTime = time;
}

void plazza::Pizza::setType(plazza::Pizza::PizzaType type)
{
    _type = type;
}

void plazza::Pizza::setName(const std::string &name)
{
    _name = name;
}

void plazza::Pizza::setIngredients(const std::vector<plazza::Pizza::PizzaIngredient> &ingredients)
{
    _ingredients = ingredients;
}
