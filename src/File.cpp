/*
** EPITECH PROJECT, 2024
** Pipe.cpp
** File description:
** Pipe.cpp
*/

#include <stdio.h>
#include <iostream>
#include "File.hpp"
#include "unistd.h"

file::Pipe::Pipe(std::string const &path, Mode mode)
{
    int m = (mode == READ ? O_RDONLY : O_WRONLY);

    if (mkfifo(path.c_str(), 0777) != 0) {
        if (errno == EEXIST)
            goto style;
        perror("mkfifo");
        throw CreateException("Failed to create pipe");
    }
style:
    this->_fd = open(path.c_str(), m);
    if (this->_fd == -1) {
        perror("open");
        throw InitException("Failed to open pipe");
    }

    this->_mode = mode;
}

void file::Pipe::destroy()
{
    close(this->_fd);
    this->_fd = -1;
}

void file::Pipe::writeSingle(char c)
{
    if (this->_mode != WRITE)
        throw InvalidModeException("Cannot write to a read-only pipe");
    write(this->_fd, &c, 1);
}

void file::Pipe::writeBuf(std::vector<char> buffer)
{
    if (this->_mode != WRITE)
        throw InvalidModeException("Cannot write to a read-only pipe");
    write(this->_fd, buffer.data(), buffer.size());
}

buffer::ByteBuf file::Pipe::readBuf()
{
    if (this->_mode != READ)
        throw InvalidModeException("Cannot read from a write-only pipe");
    std::vector<char> buffer;
    char buf[4096];
    int n;

    n = read(this->_fd, buf, 4096);
    if (n == -1) {
        perror("read");
        throw std::runtime_error("Failed to read from pipe");
    }

    if (n < 4096)
        buffer.insert(buffer.end(), buf, buf + n);
    else {
        buffer.insert(buffer.end(), buf, buf + n);
        while ((n = read(this->_fd, buf, 4096)) == 4096) {
            buffer.insert(buffer.end(), buf, buf + n);
        }
    }
    return buffer::ByteBuf(buffer);
}
