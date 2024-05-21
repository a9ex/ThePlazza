/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Main.cpp
*/

#include <unistd.h>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include "SafeQueue.hpp"
#include "ThreadPool.hpp"
#include "PizzaBuilder.hpp"
#include "PizzaOrderParser.hpp"

#ifndef CRITERION

void *printRandomNumber()
{
    std::uniform_int_distribution<std::mt19937::result_type> range(1, 9);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::cout << range(rng) << std::endl;
    return nullptr;
}

int main(void)
{
    // std::size_t i = 0;
    // ThreadPool pool(8);

    // for (; i < 1000; ++i)
    //     pool.addTask(printRandomNumber);
    // pool.run();

    plazza::PizzaBuilder builder;
    builder.setPizzaName("regina")
    .setPizzaType(plazza::Pizza::PizzaType::Regina)
    .setCookingTime(2)
    .setIngredients({plazza::Pizza::PizzaIngredient::Dough, plazza::Pizza::PizzaIngredient::Tomato, plazza::Pizza::PizzaIngredient::Gruyere, plazza::Pizza::PizzaIngredient::Ham, plazza::Pizza::PizzaIngredient::Mushrooms});

    plazza::PizzaBuilder builder2;
    builder2.setPizzaName("fantasia")
    .setPizzaType(plazza::Pizza::PizzaType::Fantasia)
    .setCookingTime(4)
    .setIngredients({plazza::Pizza::PizzaIngredient::Dough, plazza::Pizza::PizzaIngredient::Tomato, plazza::Pizza::PizzaIngredient::Gruyere, plazza::Pizza::PizzaIngredient::Mushrooms, plazza::Pizza::PizzaIngredient::Steak, plazza::Pizza::PizzaIngredient::Eggplant, plazza::Pizza::PizzaIngredient::GoatCheese, plazza::Pizza::PizzaIngredient::ChiefLove});

    plazza::Pizza pizza = builder.build();
    plazza::Pizza pizza2 = builder2.build();
    std::map<std::string, plazza::Pizza> pizzas;
    pizzas["regina"] = pizza;
    pizzas["fantasia"] = pizza2;

    plazza::PizzaOrderParser parser(pizzas);

    try {
        std::vector<plazza::PizzaOrderParser::PizzaOrder> orders = parser.parsePizzaOrder("MALON DURO XXL x2; fantasia M x3a; fantasia X x2; fantasia XL 2; regina XXL x3");
    } catch (const plazza::PizzaParserException &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

#endif
