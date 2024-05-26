/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Input.cpp
*/

#include <iostream>
#include <thread>
#include "Input.hpp"

void plazza::Input::handleUserInput(
    plazza::Holders &holders,
    std::map<std::string, plazza::Pizza> &pizzas,
    std::vector<std::shared_ptr<plazza::Kitchen>> &kitchens,
    std::mutex &mutex
)
{
    std::string input;
    while (true) {
        std::osyncstream(std::cout) << "Benvenuti alla Pappa's Pizzeria ! I'm Mario, what can I do for you ? (enter your order): ";
        std::getline(std::cin, input);

        mutex.lock();
        if (input == "exit") {
            std::osyncstream(std::cout) << "Thanks for visiting our Pappa's Pizzeria ! Babaye !" << std::endl;
            exit(0);
        }

        plazza::PizzaOrderParser parser(pizzas);
        std::vector<plazza::PizzaOrderParser::PizzaOrder> orders;

        try {
            orders = parser.parsePizzaOrders(input);
        } catch (plazza::PizzaParserException &e) {
            std::cerr << "[PizzaOrderParser] " << e.what() << std::endl;
            continue;
        }

        for (auto &order : orders) {
            plazza::Pizza pizza = plazza::Pizza::getPizzaByType(order.type, pizzas);
            pizza.setSize(order.size);

            plazza::PizzaBalancer balancer;
            auto chosenKitchen = balancer.balancePizza(pizza, kitchens);

            if (!chosenKitchen) {
                std::osyncstream(std::cout) << "No kitchen available! Creating a new one" << std::endl;
                plazza::KitchenSpec spec(std::to_string(kitchens.size()), 3);
                kitchens.push_back(std::make_shared<plazza::Kitchen>(holders, spec));
                chosenKitchen = kitchens.back();
            }

            *chosenKitchen << pizza;
        }
        mutex.unlock();
    }
}
