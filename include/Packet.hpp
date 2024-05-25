/*
** EPITECH PROJECT, 2024
** Packet.hpp
** File description:
** Packet.hpp
*/

#pragma once

#include "File.hpp"
#include "Buffer.hpp"
#include "Pizza.hpp"
#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <iostream>
#include <syncstream>

namespace comm {
    class Packet {
    public:
        enum Type {
            PING = 0x01,
            KITCHEN_REFILL,
            PIZZA_ORDER,
            PIZZA_READY,
            PIZZA_CHANGE_STATUS,
        };
    public:
        Packet(Type type) : _type(type) {}
        ~Packet() = default;

        virtual buffer::ByteBuf serialize() const = 0;
        virtual void deserialize(buffer::ByteBuf &buffer) = 0;

        Packet &operator<<(buffer::ByteBuf &buffer);
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

        void deserialize(buffer::ByteBuf &buff) override {
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

        void deserialize(buffer::ByteBuf &buff) override {
        }
    };

    class PizzaOrderPacket : public Packet {
    public:
        PizzaOrderPacket() : Packet(PIZZA_ORDER) {}
        PizzaOrderPacket(unsigned long id, plazza::Pizza pizza) : Packet(PIZZA_ORDER), _id(id), _pizza(pizza) {}
        ~PizzaOrderPacket() = default;

        buffer::ByteBuf serialize() const override {
            buffer::ByteBuf buffer;

            buffer.writeUnsignedLong(this->_id);
            buffer.writeLong(this->_pizza.getType());
            buffer.writeDouble(this->_pizza.getCookingTime());
            buffer.writeString(this->_pizza.getName());
            buffer.writeInt(this->_pizza.getSize());
            buffer.writeUnsignedLong(this->_pizza.getIngredients().size());
            for (auto &ingredient : this->_pizza.getIngredients()) {
                buffer.writeInt(ingredient);
            }

            return buffer;
        }

        void deserialize(buffer::ByteBuf &buff) override {
            this->_id = buff.readUnsignedLong();
            this->_pizza.setType((plazza::PizzaType) buff.readLong());
            this->_pizza.setCookingTime(buff.readDouble());
            this->_pizza.setName(buff.readString());
            this->_pizza.setSize((plazza::PizzaSize) buff.readInt());
            std::size_t ingredients_size = buff.readUnsignedLong();
            std::vector<plazza::PizzaIngredient> ingredients;
            for (std::size_t i = 0; i < ingredients_size; i++) {
                ingredients.push_back((plazza::PizzaIngredient) buff.readInt());
            }
            this->_pizza.setIngredients(ingredients);
        }

        unsigned long getId() const { return this->_id; }
        plazza::Pizza getPizza() const { return this->_pizza; }

    private:
        unsigned long _id;
        plazza::Pizza _pizza;
    };

    class PizzaReadyPacket : public Packet {
    public:
        PizzaReadyPacket() : Packet(PIZZA_READY) {}
        PizzaReadyPacket(unsigned long id) : Packet(PIZZA_READY), _id(id) {}
        ~PizzaReadyPacket() = default;

        buffer::ByteBuf serialize() const override {
            buffer::ByteBuf buffer;

            buffer.writeUnsignedLong(this->_id);

            return buffer;
        }

        void deserialize(buffer::ByteBuf &buff) override {
            this->_id = buff.readUnsignedLong();
        }

        unsigned long getId() const { return this->_id; }

    private:
        int _id = 100;
    };

    class PizzaChangeStatusPacket : public Packet {
    public:
        PizzaChangeStatusPacket() : Packet(PIZZA_CHANGE_STATUS) {}
        PizzaChangeStatusPacket(unsigned long id, bool status) : Packet(PIZZA_CHANGE_STATUS), _id(id), _status(status) {}
        ~PizzaChangeStatusPacket() = default;

        buffer::ByteBuf serialize() const override {
            buffer::ByteBuf buffer;

            buffer.writeUnsignedLong(this->_id);
            buffer.writeInt(this->_status ? 1 : 0);

            return buffer;
        }

        void deserialize(buffer::ByteBuf &buff) override {
            this->_id = buff.readUnsignedLong();
            this->_status = buff.readInt();
        }

        unsigned long getId() const { return this->_id; }
        bool getStatus() const { return this->_status ? true : false; }

    private:
        unsigned long _id;
        bool _status;
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
                    std::osyncstream(std::cout) << "Unknown packet type '" << type << "'" << std::endl;
                }
            }
            return packets;
        }
    private:
        std::map<Packet::Type, std::function<std::shared_ptr<Packet>()>> _packet_constructors = {
            {Packet::PING, []() { return std::make_shared<PingPacket>(); }},
            {Packet::KITCHEN_REFILL, []() { return std::make_shared<KitchenRefillPacket>(); }},
            {Packet::PIZZA_ORDER, []() { return std::make_shared<PizzaOrderPacket>(); }},
            {Packet::PIZZA_READY, []() { return std::make_shared<PizzaReadyPacket>(); }},
            {Packet::PIZZA_CHANGE_STATUS, []() { return std::make_shared<PizzaChangeStatusPacket>(); }},
        };
    };
};
