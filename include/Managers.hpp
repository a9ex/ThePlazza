/*
** EPITECH PROJECT, 2024
** Managers.hpp
** File description:
** Managers.hpp
*/

#pragma once

#include <thread>
#include <filesystem>
#include <vector>
#include <algorithm>

namespace plazza {
    template <typename T>
    class Manager {
    public:
        Manager() = default;
        ~Manager() = default;

        void add(T object) { this->_objects.push_back(object); }
        void remove(T object) {
            this->_objects.erase(
                std::remove(this->_objects.begin(), this->_objects.end(), object),
                this->_objects.end());
        }
        std::vector<T> getObjects() const { return this->_objects; }

        void closeAllExcept(T obj) {
            for (auto &object : this->_objects)
                if (object != obj)
                    object.join();
        }
    private:
        std::vector<T> _objects;
    };

    class FileManager : public Manager<int> {};

    class ThreadManager : public Manager<std::thread> {};
}
