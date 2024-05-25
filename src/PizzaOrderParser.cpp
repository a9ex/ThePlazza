#include "PizzaOrderParser.hpp"
#include <algorithm>
#include <sstream>
#include <iostream>

plazza::PizzaOrderParser::PizzaOrderParser(std::map<std::string, Pizza> &pizzas)
    : _pizzas(pizzas)
{
}

plazza::PizzaParserException::PizzaParserException(const std::string &message)
    : _message(message)
{
}

const char *plazza::PizzaParserException::what() const noexcept
{
    return _message.c_str();
}

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
        return str;

    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

plazza::PizzaType plazza::PizzaOrderParser::_parseType(std::string &type)
{
    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    if (_pizzas.find(type) == _pizzas.end()) {
        throw PizzaParserException("Unknown pizza type: " + type);
    }

    return _pizzas[type].getType();
}

plazza::PizzaSize plazza::PizzaOrderParser::_parseSize(std::string &size)
{
    std::transform(size.begin(), size.end(), size.begin(), ::toupper);
    if (_sizes.find(size) == _sizes.end()) {
        throw PizzaParserException("Unknown pizza size: " + size);
    }

    return _sizes[size];
}

uint32_t plazza::PizzaOrderParser::_parseQuantity(const std::string &quantity)
{
    if (quantity.at(0) != 'x')
        throw PizzaParserException("Invalid quantity format - where is the 'x'?");
    if (quantity.find_first_not_of("0123456789", 1) != std::string::npos)
        throw PizzaParserException("Invalid quantity format - not a number: " + quantity);
    try {
        return std::stoul(quantity.substr(1));
    } catch (const std::exception &e) {
        throw PizzaParserException("Invalid quantity format: " + quantity);
    }
}

std::vector<plazza::PizzaOrderParser::PizzaOrder> plazza::PizzaOrderParser::parsePizzaOrders(const std::string &line)
{
    std::vector<PizzaOrder> orders;
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(line);

    std::cout << "[PizzaOrderParser] Parsing order: " << line << std::endl;

    while (std::getline(tokenStream, token, ';')) {
        std::cout << "[PizzaOrderParser] Got token: " << trim(token) << std::endl;
        tokens.push_back(trim(token));
    }

    for (const auto &order : tokens) {
        std::vector<std::string> orderTokens;
        std::string orderToken;
        std::istringstream orderTokenStream(order);

        while (std::getline(orderTokenStream, orderToken, ' ')) {
            orderTokens.push_back(orderToken);
        }

        if (orderTokens.size() != 3) {
            // Not throwing an exception here, we want to continue processing the other orders
            std::cerr << "[PizzaOrderParser] The following order is invalid and will be ignored: " << order << " (invalid number of tokens) continuing processing..." << std::endl;
            continue;
        }

        PizzaOrder pizzaOrder;
        int quantity = 0;

        try {
            pizzaOrder.type = _parseType(orderTokens[0]);
            pizzaOrder.size = _parseSize(orderTokens[1]);
            quantity = _parseQuantity(orderTokens[2]);
        } catch (const PizzaParserException &e) {
            std::cerr << "[PizzaOrderParser] The following order is invalid and will be ignored: " << order << " (" << e.what() << ") continuing processing..." << std::endl;
            continue;
        }

        for (int i = 0; i < quantity; i++) {
            orders.push_back(pizzaOrder);
            std::cout << "[PizzaOrderParser] Parsed order: Adding pizza of type " << orderTokens[0] << " and size " << orderTokens[1] << ". Ready to be dispatched to the kitchens" << std::endl;
        }

    }

    return orders;
}
