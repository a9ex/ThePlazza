/*
** EPITECH PROJECT, 2024
** Packet.hpp
** File description:
** Packet.hpp
*/

#pragma once

#include "File.hpp"
#include "Buffer.hpp"
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <iostream>

namespace comm {
    class Packet {
    public:
        enum Type {
            PING = 0x00,
        };
    public:
        Packet(Type type) : _type(type) {}
        ~Packet() = default;

        virtual buffer::ByteBuf serialize() const = 0;
        virtual void deserialize(buffer::ByteBuf buffer) = 0;

        Packet &operator<<(const buffer::ByteBuf buffer);
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

        buffer::ByteBuf serialize() const override {
            buffer::ByteBuf buffer;

            buffer.writeInt(this->_i);
            return buffer;
        }

        void deserialize(buffer::ByteBuf buff) override {
            this->_i = buff.readInt();
        }

        int getI() const { return this->_i; }
    private:
        int _i;
    };

    class PacketHandler {
    public:
        PacketHandler() = default;
        ~PacketHandler() = default;

        std::unique_ptr<Packet> constructPacket(Packet::Type type) {
            return this->_packet_constructors[type]();
        }

        std::unique_ptr<Packet> constructPacket(buffer::ByteBuf buff) {
            Packet::Type type = (Packet::Type) buff.readInt();

            if (this->_packet_constructors.find(type) != this->_packet_constructors.end()) {
                auto packet = this->_packet_constructors[type]();
                *packet << buff;
                return packet;
            }
            return NULL;
        }
    private:
        std::map<Packet::Type, std::function<std::unique_ptr<Packet>()>> _packet_constructors = {
            {Packet::PING, []() { return std::make_unique<PingPacket>(); }},
        };
    };
};
