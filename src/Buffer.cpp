/*
** EPITECH PROJECT, 2024
** Buffer.cpp
** File description:
** Buffer.cpp
*/

#include "Buffer.hpp"
#include <cstring>

char buffer::ByteBuf::readChar() {
    char value = this->_buffer[0];
    this->_buffer.erase(this->_buffer.begin());
    return value;
}

int buffer::ByteBuf::readInt() {
    int value;
    std::memcpy(&value, this->_buffer.data(), sizeof(int));
    this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(int));
    return value;
}

long buffer::ByteBuf::readLong() {
    long value;
    std::memcpy(&value, this->_buffer.data(), sizeof(long));
    this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(long));
    return value;
}

float buffer::ByteBuf::readFloat() {
    float value;
    std::memcpy(&value, this->_buffer.data(), sizeof(float));
    this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(float));
    return value;
}

double buffer::ByteBuf::readDouble() {
    double value;
    std::memcpy(&value, this->_buffer.data(), sizeof(double));
    this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(double));
    return value;
}

std::string buffer::ByteBuf::readString() {
    std::string value;
    std::size_t size = this->readInt();
    for (std::size_t i = 0; i < size; i++)
        value.push_back(this->readChar());
    return value;
}

unsigned long buffer::ByteBuf::readUnsignedLong() {
    unsigned long value;
    std::memcpy(&value, this->_buffer.data(), sizeof(unsigned long));
    this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(unsigned long));
    return value;
}

void buffer::ByteBuf::writeChar(char c) {
    this->_buffer.push_back(c);
}

void buffer::ByteBuf::writeInt(int i) {
    std::vector<char> bytes(sizeof(int));
    std::memcpy(bytes.data(), &i, sizeof(int));
    this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
}

void buffer::ByteBuf::writeLong(long l) {
    std::vector<char> bytes(sizeof(long));
    std::memcpy(bytes.data(), &l, sizeof(long));
    this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
}

void buffer::ByteBuf::writeFloat(float f) {
    std::vector<char> bytes(sizeof(float));
    std::memcpy(bytes.data(), &f, sizeof(float));
    this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
}

void buffer::ByteBuf::writeDouble(double d) {
    std::vector<char> bytes(sizeof(double));
    std::memcpy(bytes.data(), &d, sizeof(double));
    this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
}

void buffer::ByteBuf::writeBuffer(std::vector<char> buffer) {
    for (auto &c : buffer)
        this->_buffer.push_back(c);
}

void buffer::ByteBuf::writeBuffer(ByteBuf buffer) {
    this->writeBuffer(buffer.getBuffer());
}

void buffer::ByteBuf::writeString(std::string str) {
    this->writeInt(str.size());
    for (auto &c : str)
        this->writeChar(c);
}

void buffer::ByteBuf::writeUnsignedLong(unsigned long l) {
    std::vector<char> bytes(sizeof(unsigned long));
    std::memcpy(bytes.data(), &l, sizeof(unsigned long));
    this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
}
