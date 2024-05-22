/*
** EPITECH PROJECT, 2024
** Packet.hpp
** File description:
** Packet.hpp
*/

#pragma once

#include "File.hpp"
#include <string>
#include <vector>

namespace comm {
    class Packet {
    public:
        enum Type {
            PING = 0x00,
        };
    public:
        Packet(Type type) : _type(type) {}
        ~Packet() = default;

        virtual std::vector<char> serialize() const = 0;
        virtual void deserialize(std::vector<char> buffer) = 0;

        Packet &operator<<(const std::vector<char> buffer);
        Packet &operator>>(file::Pipe &pipe);

        Type getType() const { return this->_type; }
    private:
        Type _type;
    };

    class PingPacket : public Packet {
    public:
        PingPacket(int i) : Packet(PING), _i(i) {}
        PingPacket() : Packet(PING), _i(0) {}
        ~PingPacket() = default;

        std::vector<char> serialize() const override {
            std::vector<char> buffer;
            buffer.push_back(this->_i);
            return buffer;
        }

        void deserialize(std::vector<char> buffer) override {
            this->_i = buffer[0];
        }

        int getI() const { return this->_i; }
    private:
        int _i;
    };
};
