/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Input.cpp
*/

#include <iostream>
#include <thread>
#include "Input.hpp"

std::string getIngredientStock(plazza::KitchenSpec &spec)
{
    std::string stock = "";
    for (int i = 0; i < plazza::PizzaIngredient::INGREDIENT_ITER_END; i++) {
        stock += "\t\t- " + plazza::Pizza::getIngredientName(static_cast<plazza::PizzaIngredient>(i)) + " x" + std::to_string(spec.getStock().getIngredient(static_cast<plazza::PizzaIngredient>(i))) + "\n";
    }
    return stock;
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
        std::osyncstream(std::cout) << "Benvenuti alla Pappa's Pizzeria ! I'm Mario, what can I do for you ? (enter your order): ";
        std::getline(std::cin, input);

        mutex.lock();
        if (input == "exit") {
            std::osyncstream(std::cout) << "Thanks for visiting our Pappa's Pizzeria ! Babaye !" << std::endl;
            exit(0);
        }

        if (input == "status") {
            std::osyncstream(std::cout) << "Mama mia ! Buongiorno ! Here's the current status of the restaurant Papa's Pizzeria :\n" << std::endl;
            std::osyncstream(std::cout) << "We have " << kitchens.size() << " kitchens opened at the moment.\n" << std::endl;
            for (auto &kitchen : kitchens) {
                std::osyncstream(std::cout) << "Kitchen " << kitchen->getSpec().getId() << " (" << kitchen->getSpec().getCookers() << " cookers) : " << (kitchen->getSpec().getOvens() == kitchen->getSpec().getCookers() * 2 ? "Waiting for commands (no pizzas) - closing soon..." : "Cooking pizzas") << std::endl;
                std::osyncstream(std::cout) << "\tCurrent ingredients stock:" << std::endl;
                std::osyncstream(std::cout) << getIngredientStock(kitchen->getSpec()) << std::endl;
                std::osyncstream(std::cout) << "\tCurrent pizzas in this kitchen (cooking or waiting to be cooked): " << kitchen->getSpec().getCookers() * 2 - kitchen->getSpec().getOvens() << std::endl;
                std::osyncstream(std::cout) << "\tOven capacity : " << kitchen->getSpec().getCookers() * 2 << std::endl;
                for (auto pizzaData = kitchen->getPizzas().begin(); pizzaData != kitchen->getPizzas().end(); pizzaData++) {
                        auto pizza = pizzaData->second;
                        std::osyncstream(std::cout) << "\t\t- " << pizza.getName() << " (size " << pizza.getSizeName() << "): " << (pizza.getIsCooking() ? "cooking" : "waiting to be cooked") << std::endl;
                }
                for (unsigned int i = 0; i < kitchen->getSpec().getOvens(); i++) {
                    std::osyncstream(std::cout) << "\t\t- Empty oven" << std::endl;
                }
                std::osyncstream(std::cout) << std::endl;
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
