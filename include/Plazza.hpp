/*
** EPITECH PROJECT, 2024
** Plazza.hpp
** File description:
** Plazza.hpp
*/

#pragma once

#include "File.hpp"
#include "Process.hpp"
#include "Managers.hpp"
#include "Packet.hpp"
#include "ThreadPool.hpp"
#include "IngredientStock.hpp"
#include "Pizza.hpp"
#include "Logger.hpp"
#include <string>
#include <optional>
#include <memory>
#include <functional>
#include <deque>
#include <thread>
#include <semaphore>
#include <map>

namespace plazza {
    class Holders {
    public:
        Holders() = default;
        ~Holders() = default;

        comm::PacketHandler &getPacketHandler() { return this->_packet_handler; }

        std::deque<std::function<void(void)>> &getMainThreadRunnables() { return this->_main_thread_runnables; }
        void addMainRunnable(std::function<void(void)> runnable) {
            this->_runnables_sem.acquire();
            this->_main_thread_runnables.push_back(runnable);
            this->_runnables_sem.release();
        }
        std::binary_semaphore &getRunnablesSem() { return this->_runnables_sem; }

        unsigned long getNextPizzaId() {
            return this->_nextPizzaId++;
        }

        void close() {
            this->_close = true;
        }
        bool isClosed() { return this->_close; }

    private:
        comm::PacketHandler _packet_handler;
        std::binary_semaphore _runnables_sem = std::binary_semaphore(1);
        std::deque<std::function<void(void)>> _main_thread_runnables;
        unsigned long _nextPizzaId = 1;
        bool _close = false;
    };

    class Managers {
    public:
        Managers();

        plazza::FileManager &getFileManager() { return this->_file_manager; }
        plazza::ThreadManager &getThreadManager() { return this->_thread_manager; }
    private:
        plazza::FileManager _file_manager;
        plazza::ThreadManager _thread_manager;
    };

    class KitchenSpec {
    public:
        KitchenSpec(
            std::string id,
            int ovens
        ) :
            _id(id),
            ovens(ovens),
            cookers(ovens)
        {}
        ~KitchenSpec() = default;

        std::string getId() { return this->_id; }
        unsigned int getOvens() { return this->ovens; }
        unsigned int getCookers() { return this->cookers; }
        void increaseOvens() { this->ovens++; }
        void decreaseOvens() { this->ovens--; }
        void doubleOvens() { this->ovens *= 2; }
        IngredientStock &getStock() { return this->_stock; }
    protected:
        std::string _id;
        unsigned int ovens;
        unsigned int cookers;
        IngredientStock _stock;
    };

    class Kitchen {
    public:
        Kitchen(Holders &holders, KitchenSpec spec);
        Kitchen(Kitchen const &) = delete;
        Kitchen(Kitchen &&) = delete;
        ~Kitchen() = default;
        void cookPizza(std::string pizza);
        void closeKitchen();

        Kitchen &operator<<(comm::Packet &packet) {
            packet >> *this->_input_pipe;
            return *this;
        }

        void print(std::string const &text) {
            plazza::Logger::printAndLog("[Kitchen " + this->_spec.getId() + "] " + text);
        }

        Kitchen &operator<<(std::string const &text) {
            plazza::Logger::printAndLog("[Kitchen " + this->_spec.getId() + "] " + text);
            return *this;
        }

        Kitchen &operator<<(Pizza &pizza);

        void updateLastCommandMillis() {
            this->_last_command_millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        bool hasEnoughIngredientsFor(Pizza &pizza) {
            for (auto &ingredient : pizza.getIngredients()) {
                if (this->_spec.getStock().getIngredient(ingredient) == 0)
                    return false;
            }
            return true;
        }

        KitchenSpec &getSpec() { return this->_spec; }
        std::map<unsigned long, Pizza> &getPizzas() { return this->_pizzas; }

        void close();
        bool isClosed() { return this->_close; }
    private:
        Holders &_holders;
        KitchenSpec _spec;
        std::unique_ptr<process::ForkProcess> _process = std::unique_ptr<process::ForkProcess>(nullptr);
        std::unique_ptr<file::Pipe> _output_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<file::Pipe> _input_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::thread _thread;
        std::thread _watcher;
        long _last_command_millis = 0;
        std::map<unsigned long, Pizza> _pizzas;
        bool _close = false;
    };

    class LocalKitchen {
    public:
        LocalKitchen(Holders &holders, KitchenSpec spec);
        ~LocalKitchen() = default;

        LocalKitchen &operator<<(comm::Packet &packet) {
            packet >> *this->_output_pipe;
            return *this;
        }

        LocalKitchen &operator<<(std::string const &text) {
            plazza::Logger::printAndLog("[LocalKitchen " + this->_spec.getId() + "] " + text);
            return *this;
        }

        void onPacketReceived(comm::Packet &packet);

        std::map<unsigned long, Pizza> &getPizzaQueue() { return this->_pizza_queue; }
        std::mutex &getPizzaQueueMutex() { return this->_pizza_queue_mutex; }

        void addPizzaToQueue(unsigned long id, Pizza pizza);

        bool hasEnoughIngredientsFor(Pizza &pizza);

        void scheduleNextPizza();

        std::mutex &getOvenMutex() { return this->_oven_mutex; }
    private:
        KitchenSpec _spec;
        std::unique_ptr<file::Pipe> _input_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<file::Pipe> _output_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<ThreadPool> _thread_pool = std::unique_ptr<ThreadPool>(nullptr);
        std::map<unsigned long, Pizza> _pizza_queue;
        std::mutex _pizza_queue_mutex;
        std::mutex _oven_mutex;
        bool _close = false;
    };

    class PlazzaContext {
    public:
        PlazzaContext();
        ~PlazzaContext() = default;

        comm::PacketHandler &getPacketHandler() { return this->_packet_handler; }
    private:
        comm::PacketHandler _packet_handler;
    };

    class PacketReceiver {
    public:
        void onReceive(Holders &holders,
            Kitchen &kitchen,
            comm::Packet &packet);
    };

    class PizzaBalancer {
    public:
        std::shared_ptr<plazza::Kitchen> balancePizza(plazza::Pizza &pizza,
            std::vector<std::shared_ptr<Kitchen>> kitchens);
    };

    // PlazzaSpecs is a singleton, to be easily accessible from anywhere
    class PlazzaSpecs {
        public:
            /**
            * @brief Remove the copy constructor and the assign operator, as it is a singleton
            */
            PlazzaSpecs(const PlazzaSpecs &) = delete;
            PlazzaSpecs &operator=(const PlazzaSpecs &) = delete;

            /**
             * @brief PlazzaSpec struct
             *
             */
            struct PlazzaSpec {
                unsigned long multiplier;
                unsigned int cooks;
                unsigned int refresh_rate;
            };

            /**
             * @brief Get the Instance object
             *
             * @return PlazzaSpecs& the instance of PlazzaSpecs
             */
            static PlazzaSpecs &getInstance() {
                static PlazzaSpecs instance;
                return instance;
            }

            /**
             * @brief Get the PlazzaSpec object
             *
             * @return PlazzaSpec& the PlazzaSpec object
             */
            PlazzaSpec &getSpec() { return this->_spec; }

            /**
             * @brief Set the Spec object
             *
             * @param spec the PlazzaSpec object to set
             */
            void setSpec(PlazzaSpec spec) { this->_spec = spec; }
        private:
            PlazzaSpecs() = default;
            ~PlazzaSpecs() = default;

            PlazzaSpec _spec;
    };
}
