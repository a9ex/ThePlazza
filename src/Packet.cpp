/*
** EPITECH PROJECT, 2024
** Packet.cpp
** File description:
** Packet.cpp
*/

#include "Packet.hpp"

// Handlers

std::vector<std::shared_ptr<comm::Packet>> comm::PacketHandler::constructPackets(buffer::ByteBuf buff) {
    std::vector<std::shared_ptr<Packet>> packets;

    while (buff.size() >= 4) {

        Packet::Type type = (Packet::Type) buff.readInt();

        if (this->_packet_constructors.find(type) != this->_packet_constructors.end()) {
            auto packet = this->_packet_constructors[type]();
            *packet << buff;
            packets.push_back(std::move(packet));
        } else {
            plazza::Logger::printAndLog("Unknown packet type '" + std::to_string(type) + "'");
        }
    }
    return packets;
}

// Serialize into a pipe

comm::Packet &comm::Packet::operator>>(file::Pipe &pipe)
{
    auto serialized = this->serialize();

    buffer::ByteBuf buffer;
    buffer.writeInt(this->getType());
    buffer.writeBuffer(serialized);
    pipe.writeBuf(buffer.getBuffer());
    return *this;
}

// Deserialize from a pipe

comm::Packet &comm::Packet::operator<<(buffer::ByteBuf &buffer)
{
    this->deserialize(buffer);
    return *this;
}

// Impl

buffer::ByteBuf comm::PingPacket::serialize() const {
    buffer::ByteBuf buffer;

    buffer.writeInt(this->_i);
    return buffer;
}

void comm::PingPacket::deserialize(buffer::ByteBuf &buff) {
    this->_i = buff.readInt();
}

 buffer::ByteBuf comm::PizzaOrderPacket::serialize() const {
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

void comm::PizzaOrderPacket::deserialize(buffer::ByteBuf &buff) {
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

buffer::ByteBuf comm::PizzaReadyPacket::serialize() const {
    buffer::ByteBuf buffer;

    buffer.writeUnsignedLong(this->_id);

    return buffer;
}

void comm::PizzaReadyPacket::deserialize(buffer::ByteBuf &buff) {
    this->_id = buff.readUnsignedLong();
}

buffer::ByteBuf comm::PizzaChangeStatusPacket::serialize() const {
    buffer::ByteBuf buffer;

    buffer.writeUnsignedLong(this->_id);
    buffer.writeInt(this->_status ? 1 : 0);

    return buffer;
}

void comm::PizzaChangeStatusPacket::deserialize(buffer::ByteBuf &buff) {
    this->_id = buff.readUnsignedLong();
    this->_status = buff.readInt();
}
