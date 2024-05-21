/*
** EPITECH PROJECT, 2024
** Concurrent Programming
** File description:
** Pizza.hpp
*/

#include <string>
#include <vector>
namespace plazza {
    // Do not modify the following enums, required in the subject
    enum PizzaType {
        Regina = 1 ,
        Margarita = 2 ,
        Americana = 4 ,
        Fantasia = 8
    };

    enum PizzaSize {
        S = 1,
        M = 2,
        L = 4,
        XL = 8,
        XXL = 16
    };

    // Can be modified
    enum PizzaIngredient {
        Dough = 1,
        Tomato,
        Gruyere,
        Ham,
        Mushrooms,
        Steak,
        Eggplant,
        GoatCheese,
        ChiefLove
    };

    // Represents a pizza
    class Pizza {
        public:
            Pizza() = default;
            ~Pizza() = default;

            double getCookingTime() const;
            PizzaType getType() const;
            std::string getName() const;
            std::vector<PizzaIngredient> getIngredients() const;

            void setCookingTime(double time);
            void setType(PizzaType type);
            void setName(const std::string &name);
            void setIngredients(const std::vector<PizzaIngredient> &ingredients);
        protected:
        private:
            double _cookingTime;
            PizzaType _type;
            std::string _name;
            std::vector<PizzaIngredient> _ingredients;
    };
}
