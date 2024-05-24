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
            PING = 0x01,
            KITCHEN_REFILL,
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

    class KitchenRefillPacket : public Packet {
    public:
        KitchenRefillPacket() : Packet(KITCHEN_REFILL) {}
        ~KitchenRefillPacket() = default;

        buffer::ByteBuf serialize() const override {
            buffer::ByteBuf buffer;

            return buffer;
        }

        void deserialize(buffer::ByteBuf buff) override {
        }
    };

    class PacketHandler {
    public:
        PacketHandler() = default;
        ~PacketHandler() = default;

        std::shared_ptr<Packet> constructPacket(Packet::Type type) {
            return this->_packet_constructors[type]();
        }

        std::vector<std::shared_ptr<Packet>> constructPackets(buffer::ByteBuf buff) {
            std::vector<std::shared_ptr<Packet>> packets;

            while (buff.size() >= 4) {

                Packet::Type type = (Packet::Type) buff.readInt();

                if (this->_packet_constructors.find(type) != this->_packet_constructors.end()) {
                    auto packet = this->_packet_constructors[type]();
                    *packet << buff;
                    packets.push_back(std::move(packet));
                } else {
                    std::cout << "Unknown packet type" << std::endl;
                }
            }
            return packets;
        }
    private:
        std::map<Packet::Type, std::function<std::shared_ptr<Packet>()>> _packet_constructors = {
            {Packet::PING, []() { return std::make_shared<PingPacket>(); }},
            {Packet::KITCHEN_REFILL, []() { return std::make_shared<KitchenRefillPacket>(); }},
        };
    };
};
