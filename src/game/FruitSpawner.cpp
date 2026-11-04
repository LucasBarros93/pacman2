#include "game/FruitSpawner.hpp"

FruitSpawner::FruitSpawner(Position spawnPosition)
    : spawnPosition(spawnPosition) {}

Fruit FruitSpawner::createFruit(int round) const {
    if (round <= 1)
        return {FruitType::Cherry, spawnPosition, 100};

    if (round == 2)
        return {FruitType::Strawberry, spawnPosition, 300};

    if (round <= 4)
        return {FruitType::Orange, spawnPosition, 500};

    if (round <= 6)
        return {FruitType::Apple, spawnPosition, 700};

    if (round <= 8)
        return {FruitType::Melon, spawnPosition, 1000};

    if (round <= 10)
        return {FruitType::Galaxian, spawnPosition, 2000};

    if (round <= 12)
        return {FruitType::Bell, spawnPosition, 3000};

    return {FruitType::Key, spawnPosition, 5000};
}

void FruitSpawner::onDotEaten(int dotsEaten, int round) {
    if (activeFruit || spawnedFruits >= 2)
        return;

    if (dotsEaten != FIRST_DOT && dotsEaten != SECOND_DOT)
        return;

    activeFruit = createFruit(round);
    remainingTime = FRUIT_DURATION;
    ++spawnedFruits;
}

void FruitSpawner::update(float deltaTime) {
    if (!activeFruit)
        return;

    remainingTime -= deltaTime;

    if (remainingTime <= 0.0f) {
        activeFruit.reset();
        remainingTime = 0.0f;
    }
}

std::optional<int> FruitSpawner::tryCollect(Position pacmanPosition) {
    if (!activeFruit.has_value()) {
        return std::nullopt;
    }

    constexpr float COLLISION_RADIUS = 0.6f;

    const float dx = pacmanPosition.first - activeFruit->position.first;
    const float dy = pacmanPosition.second - activeFruit->position.second;

    if (dx * dx + dy * dy <= COLLISION_RADIUS * COLLISION_RADIUS) {
        const int points = activeFruit->score;
        activeFruit.reset();
        remainingTime = 0.0f;
        return points;
    }

    return std::nullopt;
}

const std::optional<Fruit> &FruitSpawner::getActiveFruit() const {
    return activeFruit;
}

void FruitSpawner::resetRound() {
    activeFruit.reset();
    spawnedFruits = 0;
    remainingTime = 0.0f;
}
