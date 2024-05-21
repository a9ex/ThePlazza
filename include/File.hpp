/*
** EPITECH PROJECT, 2024
** Pipe.hpp
** File description:
** Pipe.hpp
*/

#pragma once

#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdexcept>

namespace file {
    class Pipe {
    public:
        enum Mode {
            READ,
            WRITE,
        };

        class CreateException : public std::runtime_error {
        public:
            CreateException(std::string const &message)
                : std::runtime_error(message) {}
        };

        class InitException : public std::runtime_error {
        public:
            InitException(std::string const &message)
                : std::runtime_error(message) {}
        };

        class InvalidModeException : public std::runtime_error {
        public:
            InvalidModeException(std::string const &message)
                : std::runtime_error(message) {}
        };
    public:
        Pipe(std::string const &path, Mode mode, bool init);
        ~Pipe() { this->destroy(); };

        void destroy();

        void writeSingle(char c);
        void writeBuf(std::vector<char> buffer);
        std::vector<char> readBuf();

        Pipe &operator<<(std::vector<char> buffer) {
            this->writeBuf(buffer);
            return *this;
        }

        Pipe &operator<<(std::string buffer) {
            this->writeBuf(std::vector<char>(buffer.begin(), buffer.end()));
            return *this;
        }

        Pipe &operator>>(std::vector<char> &buffer) {
            buffer = this->readBuf();
            return *this;
        }
    private:
        int _fd;
        Mode _mode;
    };
}
