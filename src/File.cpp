/*
** EPITECH PROJECT, 2024
** Pipe.cpp
** File description:
** Pipe.cpp
*/

#include "File.hpp"

file::Pipe::Pipe(std::string const &path, Mode mode, bool init)
{
    mode_t m = (mode == READ ? O_RDONLY : O_WRONLY);

    if (init)
        if (mkfifo(path.c_str(), m) != 0)
            throw CreateException("Failed to create pipe");
    this->_fd = open(path.c_str(), m);
    if (this->_fd == -1)
        throw InitException("Failed to open pipe");

    this->_mode = mode;
}

void file::Pipe::destroy()
{
    close(this->_fd);
}

void file::Pipe::writeBuf(std::vector<char> buffer)
{
    if (this->_mode != WRITE)
        throw InvalidModeException("Cannot write to a read-only pipe");
    write(this->_fd, buffer.data(), buffer.size());
}

std::vector<char> file::Pipe::readBuf()
{
    if (this->_mode != READ)
        throw InvalidModeException("Cannot read from a write-only pipe");
    std::vector<char> buffer;
    char buf[4096];
    int n;

    while ((n = read(this->_fd, buf, 4096)) > 0)
        buffer.insert(buffer.end(), buf, buf + n);
    return buffer;
}
