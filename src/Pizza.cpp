/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Pizza.cpp
*/

#include "Pizza.hpp"
#include <stdexcept>

double plazza::Pizza::getCookingTime() const
{
    return _cookingTime;
}

plazza::PizzaType plazza::Pizza::getType() const
{
    return _type;
}

std::string plazza::Pizza::getName() const
{
    return _name;
}

std::vector<plazza::PizzaIngredient> plazza::Pizza::getIngredients() const
{
    return _ingredients;
}

void plazza::Pizza::setCookingTime(double time)
{
    _cookingTime = time;
}

void plazza::Pizza::setType(plazza::PizzaType type)
{
    _type = type;
}

void plazza::Pizza::setName(const std::string &name)
{
    _name = name;
}

void plazza::Pizza::setIngredients(const std::vector<plazza::PizzaIngredient> &ingredients)
{
    _ingredients = ingredients;
}

plazza::PizzaSize plazza::Pizza::getSize() const
{
    return _size;
}

void plazza::Pizza::setSize(plazza::PizzaSize size)
{
    _size = size;
}

std::string plazza::Pizza::getSizeName() const
{
    return _sizeNames.at(_size);
}

plazza::Pizza &plazza::Pizza::getPizzaByType(plazza::PizzaType type, std::map<std::string, plazza::Pizza> &pizzas)
{
    for (auto &[name, pizza] : pizzas) {
        if (pizza.getType() == type) {
            return pizza;
        }
    }
    throw std::runtime_error("Pizza not found");
}

plazza::Pizza &plazza::Pizza::getPizzaFromId(const unsigned long &id, std::map<unsigned long, plazza::Pizza> &pizzas)
{
    for (auto &pizza : pizzas) {
        if (pizza.first == id) {
            return pizza.second;
        }
    }
    throw std::runtime_error("Pizza not found");
}

std::string plazza::Pizza::getIngredientName(plazza::PizzaIngredient ingredient)
{
    std::map<PizzaIngredient, std::string> ingredientNames = {
        {PizzaIngredient::Dough, "Dough"},
        {PizzaIngredient::Tomato, "Tomato"},
        {PizzaIngredient::Gruyere, "Gruyere"},
        {PizzaIngredient::Ham, "Ham"},
        {PizzaIngredient::Mushrooms, "Mushrooms"},
        {PizzaIngredient::Steak, "Steak"},
        {PizzaIngredient::Eggplant, "Eggplant"},
        {PizzaIngredient::GoatCheese, "GoatCheese"},
        {PizzaIngredient::ChiefLove, "ChiefLove"},
    };

    return ingredientNames.at(ingredient);
}
