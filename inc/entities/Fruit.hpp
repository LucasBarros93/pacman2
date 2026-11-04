#ifndef FRUIT_HPP
#define FRUIT_HPP

#include "core/Core.hpp"

enum class FruitType {
    Cherry = 1,
    Strawberry = 2,
    Orange = 3,
    Apple = 4,
    Melon = 5,
    Galaxian = 6,
    Bell = 7,
    Key = 8
};

struct Fruit {
    FruitType type;
    Position position;
    int score;
};

#endif
