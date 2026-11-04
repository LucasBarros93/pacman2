#include "game/FruitSpawner.hpp"

FruitSpawner::FruitSpawner(Position spawnPosition)
    : spawnPosition(spawnPosition) {}

Fruit FruitSpawner::createFruit(int round) const {
    return Fruit{fruitTypeForRound(round), spawnPosition};
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
    if (!activeFruit.has_value())
        return std::nullopt;

    if (!Collision::overlaps(pacmanPosition, activeFruit->position,
                             Collision::FRUIT_RADIUS))
        return std::nullopt;

    const int points = scoreFor(activeFruit->type);
    activeFruit.reset();
    remainingTime = 0.0f;
    return points;
}

const std::optional<Fruit> &FruitSpawner::getActiveFruit() const {
    return activeFruit;
}

void FruitSpawner::resetRound() {
    activeFruit.reset();
    spawnedFruits = 0;
    remainingTime = 0.0f;
}
