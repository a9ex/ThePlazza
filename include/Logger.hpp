/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Logger.hpp
*/

#pragma once
#include <fstream>

// Logger is a SINGLETON class !

namespace plazza {
    class Logger {
        public:
            /**
            * @brief Remove the copy constructor and the assign operator, as it is a singleton
            */
            Logger(const Logger &) = delete;
            Logger &operator=(const Logger &) = delete;

            /**
            * @brief Returns the instance of the Logger
            *
            * @return Logger& the instance of the Logger
            */
            static Logger &getInstance();

            /**
            * @brief Get the stream object
            *
            * @return std::ofstream& the stream
            */
            std::ofstream &getStream();

            /**
            * @brief Prints on cout via osyncstream and logs in a file with timestamp
            *
            * @param message the message to print and log
            */
            static void printAndLog(const std::string &message, bool noEndl = false, bool logOnly = false);
        private:
            Logger()
            {
                _stream.open("plazza.log", std::ios::out | std::ios::trunc);
                if (!_stream.is_open())
                    throw std::runtime_error("Could not open log file");
            }

            ~Logger()
            {
                if (_stream.is_open())
                    _stream.close();
            }

            std::ofstream _stream;
    };
}
