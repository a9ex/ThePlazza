/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** PizzaBuilder.cpp
*/

#include <iostream>
#include <syncstream>
#include "PizzaBuilder.hpp"
#include "Logger.hpp"

plazza::PizzaBuilder &plazza::PizzaBuilder::setPizzaType(PizzaType type)
{
    // std::osyncstream(std::cout) << "[PizzaBuilder] Creating pizza of type " << type << std::endl;
    _pizza.setType(type);
    return *this;
}

plazza::PizzaBuilder &plazza::PizzaBuilder::setPizzaName(const std::string &name)
{
    // std::osyncstream(std::cout) << "[PizzaBuilder] Oh mama ! Creating pizza with name " << name << std::endl;
    _pizza.setName(name);
    return *this;
}

plazza::PizzaBuilder &plazza::PizzaBuilder::setCookingTime(double time)
{
    // std::osyncstream(std::cout) << "[PizzaBuilder] Setting cooking time to " << time << std::endl;
    _pizza.setCookingTime(time);
    return *this;
}

plazza::PizzaBuilder &plazza::PizzaBuilder::setIngredients(const std::vector<PizzaIngredient> &ingredients)
{
    // std::osyncstream(std::cout) << "[PizzaBuilder] Mamamia ! Aggiungere i seguenti ingredienti alla mia super pizza" << std::endl;

    _pizza.setIngredients(ingredients);
    return *this;
}

plazza::Pizza plazza::PizzaBuilder::build()
{
    plazza::Logger::printAndLog("[PizzaBuilder] Mamamia, builded " + _pizza.getName() + " pizza");
    return _pizza;
}

plazza::PizzaBuilder &plazza::PizzaBuilder::addPizzaToMap(std::map<std::string, plazza::Pizza> &pizzas)
{
    pizzas[_pizza.getName()] = this->build();
    return *this;
}
