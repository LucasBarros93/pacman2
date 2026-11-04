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

inline FruitType fruitTypeForRound(int round) {
    switch (round) {
    case 1:
        return FruitType::Cherry;
    case 2:
        return FruitType::Strawberry;
    case 3:
    case 4:
        return FruitType::Orange;
    case 5:
    case 6:
        return FruitType::Apple;
    case 7:
    case 8:
        return FruitType::Melon;
    case 9:
    case 10:
        return FruitType::Galaxian;
    case 11:
    case 12:
        return FruitType::Bell;
    default:
        return FruitType::Key;
    }
}

inline int scoreFor(FruitType type) {
    switch (type) {
    case FruitType::Cherry:
        return 100;
    case FruitType::Strawberry:
        return 300;
    case FruitType::Orange:
        return 500;
    case FruitType::Apple:
        return 700;
    case FruitType::Melon:
        return 1000;
    case FruitType::Galaxian:
        return 2000;
    case FruitType::Bell:
        return 3000;
    case FruitType::Key:
        return 5000;
    }
    return 0;
}

struct Fruit {
    FruitType type;
    Position position;
};

#endif
