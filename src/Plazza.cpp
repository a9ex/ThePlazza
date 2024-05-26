/*
** EPITECH PROJECT, 2024
** Plazza.cpp
** File description:
** Plazza.cpp
*/

#include "Plazza.hpp"
#include "Packet.hpp"
#include <iostream>
#include <thread>
#include <chrono>

plazza::Kitchen::Kitchen(plazza::Holders &holders, plazza::KitchenSpec spec)
    : _holders(holders), _spec(spec)
{
    this->_process = std::make_unique<process::ForkProcess>([this, &holders] {
        LocalKitchen local_kitchen(holders, this->_spec);
    });

    // After creating the LocalKitchen, we double the amount of ovens (double the capacity)
    this->_spec.doubleOvens();

    plazza::Logger::printAndLog("Creating Kitchen named '" + this->_spec.getId() + "'");

    // Create pipes
    this->_input_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_input",
        file::Pipe::Mode::WRITE);

    this->_output_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_output",
        file::Pipe::Mode::READ
    );

    this->_thread = std::thread([this, &holders] {
        while (!this->_close) {
            std::vector<std::shared_ptr<comm::Packet>> packets = holders.getPacketHandler().constructPackets(this->_output_pipe->readBuf());
            if (!packets.empty()) {
                holders.addMainRunnable([this, packets, &holders] { // Capture 'this' in the lambda capture list
                    for (auto &packet : packets)
                        PacketReceiver().onReceive(holders, *this, *packet);
                });
            }
        }
    });
    this->_thread.detach();

    this->_watcher = std::thread([this] {
        while (true) {
            if (this->_close)
                break;

            // If kitchen has actual orders, we should not close it
            if (this->getPizzas().size() > 0)
                continue;
            if (this->_last_command_millis != 0 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - this->_last_command_millis > 5000) {
                this->close();
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });
    this->_watcher.detach();
}

plazza::LocalKitchen::LocalKitchen(plazza::Holders &holders, plazza::KitchenSpec spec)
    : _spec(spec)
{
    // Init pipes
    this->_input_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_input",
        file::Pipe::Mode::READ
    );

    this->_output_pipe = std::make_unique<file::Pipe>(
        "kitchen_pipe_" + this->_spec.getId() + "_output",
        file::Pipe::Mode::WRITE
    );

    // Init thread pool
    size_t thread_count = this->_spec.getOvens();

    // We need to add 1 thread for the reception dispatcher
    // thread_count++;

    // And another thread for the stock refill
    thread_count++;

    this->_thread_pool = std::make_unique<ThreadPool>(thread_count);
    this->_thread_pool->run();

    // this->_thread_pool->addTask([this] {
    //     comm::PacketHandler packet_handler;
    //     while (true) {
    //         std::vector<std::shared_ptr<comm::Packet>> packets = packet_handler.constructPackets(this->_input_pipe->readBuf());
    //         if (!packets.empty()) {
    //             std::osyncstream(std::cout) << "LocalKitchen received packets" << std::endl;
    //             for (auto &packet : packets)
    //                 this->onPacketReceived(*packet);
    //         }
    //     }
    // });

    this->_thread_pool->addTask([this] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            this->_spec.getStock().refillAll();
            comm::KitchenRefillPacket() >> *this->_output_pipe;
            // std::osyncstream(std::cout) << "Refill sent" << std::endl;
        }
    });

    // Wait 1s
    // std::this_thread::sleep_for(std::chrono::seconds(1));

    comm::PacketHandler packet_handler;
    while (true) {
        std::vector<std::shared_ptr<comm::Packet>> packets = packet_handler.constructPackets(this->_input_pipe->readBuf());
        if (!packets.empty()) {
            for (auto &packet : packets)
                this->onPacketReceived(*packet);
        }
    }

    this->_thread_pool->close(false);

    plazza::Logger::printAndLog("Fork finished");
}

void plazza::PacketReceiver::onReceive(plazza::Holders &holders,
    plazza::Kitchen &kitchen,
    comm::Packet &p)
{
   // If packet is subtype of PingPacket
   if (comm::Packet::Type::KITCHEN_REFILL == p.getType()) {
        comm::KitchenRefillPacket packet = dynamic_cast<comm::KitchenRefillPacket &>(p);
        kitchen.getSpec().getStock().refillAll();
   }

   if (comm::Packet::Type::PIZZA_READY == p.getType()) {
        comm::PizzaReadyPacket packet = dynamic_cast<comm::PizzaReadyPacket &>(p);
        plazza::Pizza pizza = plazza::Pizza::getPizzaFromId(packet.getId(), kitchen.getPizzas());

        kitchen.print("Pizza " + pizza.getName() + " (size " + pizza.getSizeName() + ") is ready !");
        kitchen.getSpec().increaseOvens();

        // Remove pizza from kitchen
        // Update last order time
        kitchen.updateLastCommandMillis();
        kitchen.getPizzas().erase(packet.getId());
   }

    if (comm::Packet::Type::PIZZA_CHANGE_STATUS == p.getType()) {
        comm::PizzaChangeStatusPacket packet = dynamic_cast<comm::PizzaChangeStatusPacket &>(p);
        plazza::Pizza &pizza = plazza::Pizza::getPizzaFromId(packet.getId(), kitchen.getPizzas());
        pizza.setIsCooking(packet.getStatus());
    }
}

void plazza::LocalKitchen::onPacketReceived(comm::Packet &packet)
{
    if (comm::Packet::Type::PIZZA_ORDER == packet.getType()) {
        comm::PizzaOrderPacket pizza_order_packet = dynamic_cast<comm::PizzaOrderPacket &>(packet);

        this->_thread_pool->addTask([this, pizza_order_packet] {
            *this << "Cooking pizza " + pizza_order_packet.getPizza().getName() + " (size " + pizza_order_packet.getPizza().getSizeName() + ")";
            comm::PizzaChangeStatusPacket(pizza_order_packet.getId(), true) >> *this->_output_pipe;
            std::this_thread::sleep_for(std::chrono::milliseconds(((long) (pizza_order_packet.getPizza().getCookingTime() * 1000))));
            comm::PizzaReadyPacket(pizza_order_packet.getId()) >> *this->_output_pipe;
        });
    }
}

std::shared_ptr<plazza::Kitchen> plazza::PizzaBalancer::balancePizza(plazza::Pizza &pizza,
    std::vector<std::shared_ptr<plazza::Kitchen>> kitchens)
{
    std::shared_ptr<plazza::Kitchen> kitchen{nullptr};

    // Remove closed kitchens
    kitchens.erase(std::remove_if(kitchens.begin(), kitchens.end(), [](std::shared_ptr<plazza::Kitchen> &kitchen) {
        return kitchen->isClosed();
    }), kitchens.end());

    // Remove kitchens that are not available
    kitchens.erase(std::remove_if(kitchens.begin(), kitchens.end(), [](std::shared_ptr<plazza::Kitchen> &kitchen) {
        return kitchen->getSpec().getOvens() == 0;
    }), kitchens.end());

    // Sort by kitchen according to the greatest ingredient number of the least
    // represented ingredient required to cook a pizza. Sorts in descending
    // order.
    std::sort(kitchens.begin(), kitchens.end(), [&pizza](std::shared_ptr<plazza::Kitchen> &a, std::shared_ptr<plazza::Kitchen> &b) {
        std::vector<std::size_t> aIngredients{0};
        std::vector<std::size_t> bIngredients{0};
        std::size_t aMin = 0;
        std::size_t bMin = 0;

        for (const plazza::PizzaIngredient &i : pizza.getIngredients()) {
            aIngredients.push_back(a->getSpec().getStock().getIngredient(i));
            bIngredients.push_back(b->getSpec().getStock().getIngredient(i));
        }
        aMin = *std::min_element(aIngredients.begin(), aIngredients.end());
        bMin = *std::min_element(bIngredients.begin(), bIngredients.end());
        return aMin > bMin;
    });

    // Get the first kitchen
    if (!kitchens.empty())
        kitchen = kitchens.front();

    return kitchen;
}
