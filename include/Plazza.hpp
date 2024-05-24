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
#include "Pizza.hpp"
#include <string>
#include <optional>
#include <memory>
#include <functional>
#include <deque>
#include <thread>
#include <semaphore>

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

    private:
        comm::PacketHandler _packet_handler;
        std::binary_semaphore _runnables_sem = std::binary_semaphore(1);
        std::deque<std::function<void(void)>> _main_thread_runnables;
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
            ovens(ovens)
        {}
        ~KitchenSpec() = default;

        std::string getId() { return this->_id; }
        int getOvens() { return this->ovens; }
        void increaseOvens() { this->ovens++; }
        void decreaseOvens() { this->ovens--; }
    protected:
        std::string _id;
        int ovens;
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

        KitchenSpec getSpec() { return this->_spec; }
    private:
        KitchenSpec _spec;
        std::unique_ptr<process::ForkProcess> _process = std::unique_ptr<process::ForkProcess>(nullptr);
        std::unique_ptr<file::Pipe> _output_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<file::Pipe> _input_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::thread _thread;
    };

    class LocalKitchen {
    public:
        LocalKitchen(Holders &holders, KitchenSpec spec);
        ~LocalKitchen() = default;

        LocalKitchen &operator<<(comm::Packet &packet) {
            packet >> *this->_output_pipe;
            return *this;
        }

        void onPacketReceived(comm::Packet &packet);
    private:
        KitchenSpec _spec;
        std::unique_ptr<file::Pipe> _input_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<file::Pipe> _output_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<ThreadPool> _thread_pool = std::unique_ptr<ThreadPool>(nullptr);
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
}
