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

        char readChar() {
            char value = this->_buffer[0];
            this->_buffer.erase(this->_buffer.begin());
            return value;
        }

        int readInt() {
            int value;
            std::memcpy(&value, this->_buffer.data(), sizeof(int));
            this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(int));
            return value;
        }

        long readLong() {
            long value;
            std::memcpy(&value, this->_buffer.data(), sizeof(long));
            this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(long));
            return value;
        }

        float readFloat() {
            float value;
            std::memcpy(&value, this->_buffer.data(), sizeof(float));
            this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(float));
            return value;
        }

        double readDouble() {
            double value;
            std::memcpy(&value, this->_buffer.data(), sizeof(double));
            this->_buffer.erase(this->_buffer.begin(), this->_buffer.begin() + sizeof(double));
            return value;
        }

        void writeChar(char c) {
            this->_buffer.push_back(c);
        }

        void writeInt(int i) {
            std::vector<char> bytes(sizeof(int));
            std::memcpy(bytes.data(), &i, sizeof(int));
            this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
        }

        void writeLong(long l) {
            std::vector<char> bytes(sizeof(long));
            std::memcpy(bytes.data(), &l, sizeof(long));
            this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
        }

        void writeFloat(float f) {
            std::vector<char> bytes(sizeof(float));
            std::memcpy(bytes.data(), &f, sizeof(float));
            this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
        }

        void writeDouble(double d) {
            std::vector<char> bytes(sizeof(double));
            std::memcpy(bytes.data(), &d, sizeof(double));
            this->_buffer.insert(this->_buffer.end(), bytes.begin(), bytes.end());
        }

        void writeBuffer(std::vector<char> buffer) {
            for (auto &c : buffer)
                this->_buffer.push_back(c);
        }

        void writeBuffer(ByteBuf buffer) {
            this->writeBuffer(buffer.getBuffer());
        }
    private:
        std::vector<char> _buffer;
    };
}
