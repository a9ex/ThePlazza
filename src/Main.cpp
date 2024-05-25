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

int main(void)
{
    plazza::Holders holders;

    int numberOfKitchens = 3;
    unsigned int ovensPerKitchen = 3;
    std::vector<std::shared_ptr<plazza::Kitchen>> kitchens;
    // for (int i = 0; i < numberOfKitchens; i++) {
    //     plazza::KitchenSpec spec(std::to_string(i), 3);
    //     kitchens.push_back(std::make_shared<plazza::Kitchen>(holders, spec));
    // }

    auto pizza = plazza::PizzaBuilder()
            .setCookingTime(1)
            .setIngredients({plazza::PizzaIngredient::ChiefLove, plazza::PizzaIngredient::Gruyere})
            .setPizzaName("Miam hannn")
            .setPizzaType(plazza::PizzaType::Regina)
            .build();

    for (int i = 0; i < 100; i++) {
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
