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
#include "Input.hpp"
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
        .addPizzaToMap(pizzas);

    plazza::PizzaBuilder()
        .setCookingTime(2)
        .setPizzaName("regina")
        .setIngredients({plazza::PizzaIngredient::Dough, plazza::PizzaIngredient::Tomato, plazza::PizzaIngredient::Gruyere, plazza::PizzaIngredient::Ham, plazza::PizzaIngredient::Mushrooms})
        .setPizzaType(plazza::PizzaType::Regina)
        .addPizzaToMap(pizzas);

    plazza::PizzaBuilder()
        .setCookingTime(2)
        .setPizzaName("americana")
        .setIngredients({plazza::PizzaIngredient::Dough, plazza::PizzaIngredient::Tomato, plazza::PizzaIngredient::Gruyere, plazza::PizzaIngredient::Steak})
        .setPizzaType(plazza::PizzaType::Americana)
        .addPizzaToMap(pizzas);

    plazza::PizzaBuilder()
        .setCookingTime(4)
        .setPizzaName("fantasia")
        .setIngredients({plazza::PizzaIngredient::Dough, plazza::PizzaIngredient::Tomato, plazza::PizzaIngredient::Eggplant, plazza::PizzaIngredient::GoatCheese, plazza::PizzaIngredient::ChiefLove})
        .setPizzaType(plazza::PizzaType::Fantasia)
        .addPizzaToMap(pizzas);
}

int main(void)
{
    plazza::Holders holders;
    std::map<std::string, plazza::Pizza> pizzas;

    int numberOfKitchens = 3;
    unsigned int ovensPerKitchen = 3;
    std::vector<std::shared_ptr<plazza::Kitchen>> kitchens;

    addRestaurantPizzas(pizzas);

    std::mutex mutex;

    std::thread userInputThread(plazza::Input::handleUserInput, std::ref(holders), std::ref(pizzas), std::ref(kitchens), std::ref(mutex));
    userInputThread.detach();

    auto &runnabled_queue = holders.getMainThreadRunnables();
    while (!holders.isClosed()) {
        mutex.lock();
        holders.getRunnablesSem().acquire();
        while (runnabled_queue.size() > 0) {
            runnabled_queue.front()();
            runnabled_queue.pop_front();
        }
        holders.getRunnablesSem().release();
        mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Close kitchens
    for (auto &kitchen : kitchens) {
        kitchen->close();
    }

    return 0;
}

#endif
