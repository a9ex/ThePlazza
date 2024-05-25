/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Main.cpp
*/

#include <iostream>
#include "Process.hpp"
#include "File.hpp"
#include "Plazza.hpp"
#include "ThreadPool.hpp"
#include "Packet.hpp"
#include "Pizza.hpp"
#include "PizzaBuilder.hpp"
#include <thread>
#include <chrono>

#ifndef CRITERION

void addRestaurantPizzas(std::map<std::string, plazza::Pizza> &pizzas)
{
    plazza::PizzaBuilder()
    .setCookingTime(1)
    .setIngredients({plazza::PizzaIngredient::Dough, plazza::PizzaIngredient::Tomato, plazza::PizzaIngredient::Gruyere})
    .setPizzaName("margarita")
    .setPizzaType(plazza::PizzaType::Margarita)
    .addToMap(pizzas);

    plazza::PizzaBuilder()
    .setCookingTime(2)
    .setPizzaName("regina")
    .setIngredients({plazza::PizzaIngredient::Dough, plazza::PizzaIngredient::Tomato, plazza::PizzaIngredient::Gruyere, plazza::PizzaIngredient::Ham, plazza::PizzaIngredient::Mushrooms})
    .setPizzaType(plazza::PizzaType::Regina)
    .addToMap(pizzas);

    plazza::PizzaBuilder()
    .setCookingTime(2)
    .setPizzaName("americana")
    .setIngredients({plazza::PizzaIngredient::Dough, plazza::PizzaIngredient::Tomato, plazza::PizzaIngredient::Gruyere, plazza::PizzaIngredient::Steak})
    .setPizzaType(plazza::PizzaType::Americana)
    .addToMap(pizzas);

    plazza::PizzaBuilder()
    .setCookingTime(4)
    .setPizzaName("fantasia")
    .setIngredients({plazza::PizzaIngredient::Dough, plazza::PizzaIngredient::Tomato, plazza::PizzaIngredient::Eggplant, plazza::PizzaIngredient::GoatCheese, plazza::PizzaIngredient::ChiefLove})
    .setPizzaType(plazza::PizzaType::Fantasia)
    .addToMap(pizzas);
}

void handleInput(plazza::Holders &holders, std::map<std::string, plazza::Pizza> &pizzas, std::vector<std::shared_ptr<plazza::Kitchen>> &kitchens)
{
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") {
            std::cout << "Thanks for coming at our Pappa's Pizzeria ! Babaye !" << std::endl;
            exit(0);
        }

        auto pizza = pizzas.find(input);
        if (pizza == pizzas.end()) {
            std::cout << "Pizza not found!" << std::endl;
            continue;
        }

        plazza::PizzaBalancer balancer;
        auto chosenKitchen = balancer.balancePizza(pizza->second, kitchens);

        if (!chosenKitchen) {
            std::cout << "No kitchen available! Creating a new one" << std::endl;
            plazza::KitchenSpec spec(std::to_string(kitchens.size()), 3);
            kitchens.push_back(std::make_shared<plazza::Kitchen>(holders, spec));
            chosenKitchen = kitchens.back();
        }

        *chosenKitchen << pizza->second;
    }
}

int main(void)
{
    plazza::Holders holders;
    std::map<std::string, plazza::Pizza> pizzas;

    int numberOfKitchens = 3;
    unsigned int ovensPerKitchen = 3;
    std::vector<std::shared_ptr<plazza::Kitchen>> kitchens;
    // for (int i = 0; i < numberOfKitchens; i++) {
    //     plazza::KitchenSpec spec(std::to_string(i), 3);
    //     kitchens.push_back(std::make_shared<plazza::Kitchen>(holders, spec));
    // }

    addRestaurantPizzas(pizzas);

    // create thread to getline


    for (int i = 0; i < 6; i++) {
        plazza::PizzaBalancer balancer;
        auto chosenKitchen = balancer.balancePizza(pizza, kitchens);

        if (!chosenKitchen) {
            std::cout << "No kitchen available! Creating a new one" << std::endl;
            plazza::KitchenSpec spec(std::to_string(kitchens.size()), ovensPerKitchen);
            kitchens.push_back(std::make_shared<plazza::Kitchen>(holders, spec));
            chosenKitchen = kitchens.back();
        }

        *chosenKitchen << pizza;
    }

    auto &runnabled_queue = holders.getMainThreadRunnables();
    while (true) {
        holders.getRunnablesSem().acquire();
        while (runnabled_queue.size() > 0) {
            runnabled_queue.front()();
            runnabled_queue.pop_front();
        }
        holders.getRunnablesSem().release();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}

#endif
