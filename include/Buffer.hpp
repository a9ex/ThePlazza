/*
** EPITECH PROJECT, 2024
** Buffer.hpp
** File description:
** Buffer.hpp
*/

#pragma once

#include <vector>
#include <cstring>
#include <iostream>

namespace buffer {
    class ByteBuf {
    public:
        ByteBuf() = default;
        ByteBuf(std::vector<char> buffer) : _buffer(buffer) {}
        ~ByteBuf() = default;

        std::size_t size() const { return this->_buffer.size(); }
        std::vector<char> getBuffer() const { return this->_buffer; }

        char readChar();
        int readInt();
        long readLong();
        float readFloat();
        double readDouble();
        std::string readString();
        unsigned long readUnsignedLong();
        void writeChar(char c);
        void writeInt(int i);
        void writeLong(long l);
        void writeFloat(float f);
        void writeDouble(double d);
        void writeBuffer(std::vector<char> buffer);
        void writeBuffer(ByteBuf buffer);
        void writeString(std::string str);
        void writeUnsignedLong(unsigned long l);
    private:
        std::vector<char> _buffer;
    };
}
