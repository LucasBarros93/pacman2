#ifndef FRUIT_HPP
#define FRUIT_HPP

#include "core/Core.hpp"

/// @brief The bonus items, from the cheapest to the most valuable.
/// @warning SpriteData uses the numeric value as the fruit's column in the
///          sprite sheet (guarded by a static_assert), so the order must match
///          it.
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

/// @brief Tells which fruit appears in a round.
/// @param round The round number, starting at 1.
/// @return The fruit of that round. From round 13 on it is always the key.
[[nodiscard]] constexpr FruitType fruitTypeForRound(int round) {
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

/// @brief Tells how many points a fruit is worth.
/// @param type The fruit.
/// @return The points for eating it (100 for the cherry up to 5000 for the
/// key).
[[nodiscard]] constexpr int scoreFor(FruitType type) {
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

/// @brief A bonus fruit waiting to be eaten.
struct Fruit {
    FruitType type;    ///< Which fruit it is.
    Position position; ///< Where it is, in tiles.
};

#endif
