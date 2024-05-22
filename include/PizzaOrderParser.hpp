#include "Pizza.hpp"
#include <cstdint>
#include <map>

#pragma once

namespace plazza
{
    class PizzaOrderParser {
        public:
            /**
            * @brief Construct a new Pizza Order Parser object
            *
            * @param pizzas A vector of pizzas available in our restaurant
            */
            PizzaOrderParser(std::map<std::string, Pizza> &pizzas);
            ~PizzaOrderParser() = default;

            struct PizzaOrder {
                Pizza::PizzaType type;
                Pizza::PizzaSize size;
            };

            /**
             * @brief Read an order line and parse it to a vector of PizzaOrder
             * Be careful, keep in mind that, if an order contains multiple pizzas of the same type, vector will contain multiple PizzaOrder with the same type.
             * It will be easier to manage the kitchen splitting then
             * This function itself does not throw any exception to guarranty the full processing of the line, but the _parse* functions can throw PizzaParserException
             * @param line the order line
             * @return std::vector<PizzaOrder> the parsed order, ready to be sent to the kitchens
             */
            std::vector<PizzaOrder> parsePizzaOrders(const std::string &line);

        private:
            std::map<std::string, Pizza> &_pizzas;
            std::map<std::string, Pizza::PizzaSize> _sizes = {
                {"S", Pizza::PizzaSize::S},
                {"M", Pizza::PizzaSize::M},
                {"L", Pizza::PizzaSize::L},
                {"XL", Pizza::PizzaSize::XL},
                {"XXL", Pizza::PizzaSize::XXL}
            };

            /**
             * @brief Parse the type of a pizza
             *
             * @param type The type token
             * @return Pizza::PizzaType The type of the pizza
             * @throw PizzaParserException if the type is invalid
             */
            Pizza::PizzaType _parseType(std::string &type);
            /**
             * @brief Parse the size of a pizza
             *
             * @param size The size token
             * @return Pizza::PizzaSize The size of the pizza
             * @throw PizzaParserException if the size is invalid
             */
            Pizza::PizzaSize _parseSize(std::string &size);
            /**
             * @brief Parse the quantity of a pizza
             *
             * @param quantity The quantity token
             * @return uint32_t The quantity of the pizza
             * @throw PizzaParserException if the quantity is invalid
             */
            uint32_t _parseQuantity(const std::string &quantity);
    };

    class PizzaParserException : public std::exception {
        public:
            PizzaParserException(const std::string &message);
            ~PizzaParserException() = default;
            const char *what() const noexcept override;
        private:
            std::string _message;
    };
}
