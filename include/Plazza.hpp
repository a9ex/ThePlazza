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

namespace plazza {
    class Holders {
    public:
        Holders() = default;
        ~Holders() = default;

        comm::PacketHandler &getPacketHandler() { return this->_packet_handler; }
    private:
        comm::PacketHandler _packet_handler;
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
        std::optional<process::ForkProcess> _process;
        std::optional<file::Pipe> _output_pipe = std::nullopt;
        std::optional<file::Pipe> _input_pipe = std::nullopt;
    };

    class LocalKitchen {
    public:
        LocalKitchen(Holders &holders, std::string id);
        ~LocalKitchen() = default;
    private:
        std::string _id;
        std::optional<file::Pipe> _input_pipe = std::nullopt;
        std::optional<file::Pipe> _output_pipe = std::nullopt;
    };

    class PlazzaContext {
    public:
        PlazzaContext();
        ~PlazzaContext() = default;

        comm::PacketHandler &getPacketHandler() { return this->_packet_handler; }
    private:
        comm::PacketHandler _packet_handler;
    };
}
