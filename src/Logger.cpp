/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Logger.cpp
*/

#include <iostream>
#include <string>
#include <syncstream>
#include <chrono>
#include "Logger.hpp"

plazza::Logger &plazza::Logger::getInstance()
{
    static Logger instance;
    return instance;
}

std::ofstream &plazza::Logger::getStream()
{
    return _stream;
}

void plazza::Logger::printAndLog(const std::string &message, bool noEndl, bool logOnly)
{
    auto &ofs = getInstance().getStream();
    std::osyncstream ofs_sync(ofs);
    auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string time_str = std::ctime(&time);

    time_str.pop_back(); // Remove the newline character introduced by std::ctime

    if (!logOnly)
        std::osyncstream(std::cout) << message << (noEndl ? "" : "\n");
    ofs << "[" << time_str << "] " << message << (noEndl ? "" : "\n");
    ofs_sync.flush();
}
