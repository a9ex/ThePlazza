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

void computeArguments(int ac, char **av)
{
    plazza::PlazzaSpecs::PlazzaSpec specs;

    if (ac == 2 && std::string(av[1]) == "-h") {
        std::cout << "USAGE: ./plazza [multiplier] [nb_cooks_per_kitchen] [restock_delay]" << std::endl;
        std::cout << "\t- multiplier - positive float (can be between 0 and 1 to use this as a divider): Used to multiply the cooking time of the pizzas" << std::endl;
        std::cout << "\t- nb_cooks_per_kitchen - positive integer: Number of cooks per kitchen" << std::endl;
        std::cout << "\t- restock_delay - positive integer: Time in milliseconds to restock one unit of each ingredients (per kitchen)\n" << std::endl;
        std::cout << "Futhermore, you can use 'status' to display the status of the kitchens in real time, and 'exit' to close the program gracefully." << std::endl;
        std::exit(0);
    }
    if (ac != 4) {
        std::cerr << "Bad number of args" << std::endl;
        std::exit(84);
    }
    if (std::string(av[2]).find_first_not_of("0123456789.") != std::string::npos || std::stof(av[1]) <= 0) {
        std::cerr << "Bad mutiplicator" << std::endl;
        std::exit(84);
    }
    if (std::string(av[2]).find_first_not_of("0123456789") != std::string::npos || std::stoi(av[2]) <= 0) {
        std::cerr << "Bad number of cooks" << std::endl;
        std::exit(84);
    }
    if (std::string(av[3]).find_first_not_of("0123456789") != std::string::npos || std::stoi(av[3]) <= 0) {
        std::cerr << "Bad restock time" << std::endl;
        std::exit(84);
    }

    specs.multiplier = std::stof(av[1]);
    specs.cooks = std::stoi(av[2]);
    specs.refresh_rate = std::stoi(av[3]);

    plazza::PlazzaSpecs::getInstance().setSpec(specs);
}

int main(int ac, char **av)
{
    plazza::Holders holders;
    std::map<std::string, plazza::Pizza> pizzas;
    std::vector<std::shared_ptr<plazza::Kitchen>> kitchens;

    computeArguments(ac, av);

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
