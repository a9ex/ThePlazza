/*
** EPITECH PROJECT, 2024
** Packet.cpp
** File description:
** Packet.cpp
*/

#include "Packet.hpp"

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

comm::Packet &comm::Packet::operator<<(const buffer::ByteBuf buffer)
{
    this->deserialize(buffer);
    return *this;
}
