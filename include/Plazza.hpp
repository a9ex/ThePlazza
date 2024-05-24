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
#include <string>
#include <optional>
#include <memory>
#include <functional>
#include <deque>
#include <thread>
#include <semaphore>

namespace plazza {
    class ReceptionDispatcher {
    public:
        ReceptionDispatcher() = default;
        ~ReceptionDispatcher() = default;

        void onPacketReceived(comm::Packet &packet);
    };

    class Holders {
    public:
        Holders() = default;
        ~Holders() = default;

        comm::PacketHandler &getPacketHandler() { return this->_packet_handler; }
        ReceptionDispatcher &getReceptionDispatcher() { return this->_reception_dispatcher; }

        std::deque<std::function<void(void)>> &getMainThreadRunnables() { return this->_main_thread_runnables; }
        void addMainRunnable(std::function<void(void)> runnable) {
            this->_runnables_sem.acquire();
            this->_main_thread_runnables.push_back(runnable);
            this->_runnables_sem.release();
        }
        std::binary_semaphore &getRunnablesSem() { return this->_runnables_sem; }

    private:
        comm::PacketHandler _packet_handler;
        ReceptionDispatcher _reception_dispatcher;
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

    class Kitchen {
    public:
        Kitchen(Holders &holders, std::string id);
        Kitchen(Kitchen const &) = delete;
        Kitchen(Kitchen &&) = delete;
        ~Kitchen() = default;
        void cookPizza(std::string pizza);
        void closeKitchen();

    private:
        std::string _id;
        std::unique_ptr<process::ForkProcess> _process = std::unique_ptr<process::ForkProcess>(nullptr);
        std::unique_ptr<file::Pipe> _output_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<file::Pipe> _input_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::thread _thread;
    };

    class LocalKitchen {
    public:
        LocalKitchen(Holders &holders, std::string id);
        ~LocalKitchen() = default;
    private:
        std::string _id;
        std::unique_ptr<file::Pipe> _input_pipe = std::unique_ptr<file::Pipe>(nullptr);
        std::unique_ptr<file::Pipe> _output_pipe = std::unique_ptr<file::Pipe>(nullptr);
    };

    class PlazzaContext {
    public:
        PlazzaContext();
        ~PlazzaContext() = default;

        comm::PacketHandler &getPacketHandler() { return this->_packet_handler; }
    private:
        comm::PacketHandler _packet_handler;
    };

    class KitchenDispatcher {

    };
}
