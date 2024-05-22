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
    auto len = serialized.size();

    pipe << std::to_string(this->_type);
    pipe << std::to_string(len);
    pipe << serialized;

    return *this;
}

// Deserialize from a pipe

comm::Packet &comm::Packet::operator<<(const std::vector<char> buffer)
{
    this->deserialize(buffer);
    return *this;
}
