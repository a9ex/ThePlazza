/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Input.cpp
*/

#include <iostream>
#include <thread>
#include "Input.hpp"

std::string plazza::Input::_getIngredientStock(plazza::KitchenSpec &spec)
{
    std::string stock = "";
    for (int i = 0; i < plazza::PizzaIngredient::INGREDIENT_ITER_END; i++) {
        stock += "\t\t- " + plazza::Pizza::getIngredientName(static_cast<plazza::PizzaIngredient>(i)) + " x" + std::to_string(spec.getStock().getIngredient(static_cast<plazza::PizzaIngredient>(i))) + "\n";
    }
    return stock;
}

std::string plazza::Input::_getOvenStates(std::shared_ptr<plazza::Kitchen> kitchen)
{
    std::string states = "";
    for (auto pizzaData = kitchen->getPizzas().begin(); pizzaData != kitchen->getPizzas().end(); pizzaData++) {
            auto pizza = pizzaData->second;
            states += "\t\t- " + pizza.getName() + " (size " + pizza.getSizeName() + "): " + (pizza.getIsCooking() ? "cooking" : "waiting to be cooked") + "\n";
    }
    for (unsigned int i = 0; i < kitchen->getSpec().getOvens(); i++) {
        states += "\t\t- Empty oven\n";
    }
    return states;
}

void plazza::Input::handleUserInput(
    plazza::Holders &holders,
    std::map<std::string, plazza::Pizza> &pizzas,
    std::vector<std::shared_ptr<plazza::Kitchen>> &kitchens,
    std::mutex &mutex
)
{
    std::string input;
    while (true) {
        plazza::Logger::printAndLog("Benvenuti alla Papa's Pizzeria ! I'm Mario, what can I do for you ? (enter your order): ", true);
        std::getline(std::cin, input);
        plazza::Logger::printAndLog(input, false, true);

        mutex.lock();
        if (input == "exit") {
            plazza::Logger::printAndLog("Thanks for visiting our Papa's Pizzeria ! Babaye !");
            holders.close();
            return;
        }

        if (input == "status") {
            plazza::Logger::printAndLog("Mama mia ! Buongiorno ! Here's the current status of the restaurant Papa's Pizzeria :\n");
            plazza::Logger::printAndLog("We have " + std::to_string(kitchens.size()) + " kitchens opened at the moment.\n");

            for (auto &kitchen : kitchens) {
                plazza::Logger::printAndLog("Kitchen " + kitchen->getSpec().getId() + " (" + std::to_string(kitchen->getSpec().getCookers()) + " cookers) : " + (kitchen->getSpec().getOvens() == kitchen->getSpec().getCookers() * 2 ? "Waiting for commands (no pizzas) - closing soon..." : "Cooking pizzas"));
                plazza::Logger::printAndLog("\tCurrent ingredients stock:");
                plazza::Logger::printAndLog(_getIngredientStock(kitchen->getSpec()));
                plazza::Logger::printAndLog("\tCurrent pizzas in this kitchen (cooking or waiting to be cooked): " + std::to_string(kitchen->getSpec().getCookers() * 2 - kitchen->getSpec().getOvens()));
                plazza::Logger::printAndLog("\tOven capacity : " + std::to_string(kitchen->getSpec().getCookers() * 2));
                plazza::Logger::printAndLog(_getOvenStates(kitchen));
                plazza::Logger::printAndLog("");
            }

            mutex.unlock();
            continue;
        }

        plazza::PizzaOrderParser parser(pizzas);
        std::vector<plazza::PizzaOrderParser::PizzaOrder> orders;

        try {
            orders = parser.parsePizzaOrders(input);
        } catch (plazza::PizzaParserException &e) {
            std::cerr << "[PizzaOrderParser] " << e.what() << std::endl;
            mutex.unlock();
            continue;
        }

        for (auto &order : orders) {
            plazza::Pizza pizza = plazza::Pizza::getPizzaByType(order.type, pizzas);
            pizza.setSize(order.size);

            plazza::PizzaBalancer balancer;
            auto chosenKitchen = balancer.balancePizza(pizza, kitchens);

            if (!chosenKitchen) {
                plazza::Logger::printAndLog("No kitchen available! Creating a new one");
                plazza::KitchenSpec spec(std::to_string(kitchens.size()), 3);
                kitchens.push_back(std::make_shared<plazza::Kitchen>(holders, spec));
                chosenKitchen = kitchens.back();
            }

            *chosenKitchen << pizza;
        }
        mutex.unlock();
    }
}
