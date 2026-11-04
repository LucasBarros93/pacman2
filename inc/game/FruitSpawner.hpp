#ifndef FRUITSPAWNER_HPP
#define FRUITSPAWNER_HPP

#include "core/Core.hpp"
#include "entities/Fruit.hpp"

#include <optional>

class FruitSpawner {
  private:
    static constexpr int FIRST_DOT = 70;
    static constexpr int SECOND_DOT = 140;
    static constexpr float FRUIT_DURATION = 8.0f;

    Position spawnPosition;

    std::optional<Fruit> activeFruit;

    int spawnedFruits = 0;
    float remainingTime = 0.0f;

    Fruit createFruit(int round) const;

  public:
    explicit FruitSpawner(Position spawnPosition);

    void onDotEaten(int dotsEaten, int round);
    void update(float deltaTime);

    std::optional<int> tryCollect(Position pacmanPosition);

    const std::optional<Fruit> &getActiveFruit() const;
    void resetRound();
};

#endif
