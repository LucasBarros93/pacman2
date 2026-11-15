#include "game/FruitSpawner.hpp"
#include "core/Core.hpp"

FruitSpawner::FruitSpawner(Position spawnPosition)
    : spawnPosition{spawnPosition} {}

void FruitSpawner::onDotEaten(int dotsEaten, int round) {
    if (dotsEaten != FIRST_DOT && dotsEaten != SECOND_DOT)
        return;

    activeFruit = Fruit{fruitTypeForRound(round), spawnPosition};
    remainingTime = FRUIT_DURATION;
}

void FruitSpawner::update(float deltaTime) {
    if (!activeFruit)
        return;

    remainingTime -= deltaTime;

    if (remainingTime <= 0.0f)
        activeFruit.reset();
}

std::optional<Fruit> FruitSpawner::tryCollect(Position pacmanPosition) {
    if (!activeFruit)
        return std::nullopt;

    if (!Collision::overlaps(pacmanPosition, activeFruit->position,
                             Collision::FRUIT_RADIUS))
        return std::nullopt;

    const Fruit collected = *activeFruit;
    activeFruit.reset();
    return collected;
}

const std::optional<Fruit> &FruitSpawner::getActiveFruit() const {
    return activeFruit;
}

void FruitSpawner::resetRound() { activeFruit.reset(); }
