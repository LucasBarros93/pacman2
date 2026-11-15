#ifndef FRUITSPAWNER_HPP
#define FRUITSPAWNER_HPP

#include "core/Core.hpp"
#include "entities/Fruit.hpp"
#include "maze/MazeLayout.hpp"

#include <optional>

/// @brief Decides when a bonus fruit appears and for how long.
///
/// A fruit shows up below the ghost house when the number of dots eaten in
/// the round reaches FIRST_DOT and again at SECOND_DOT. It stays for
/// FRUIT_DURATION seconds, and which fruit it is depends on the round.
class FruitSpawner {
  private:
    static constexpr int FIRST_DOT = 70;   ///< Dots eaten for the first fruit.
    static constexpr int SECOND_DOT = 170; ///< Dots eaten for the second fruit.
    static constexpr float FRUIT_DURATION = 8.0f; ///< Seconds a fruit stays.

    Position spawnPosition;           ///< Where the fruits appear, in tiles.
    std::optional<Fruit> activeFruit; ///< The fruit on screen, if any.
    float remainingTime = 0.0f; ///< Seconds until the active fruit disappears.

  public:
    /// @param spawnPosition Where the fruits appear, in tiles.
    explicit FruitSpawner(Position spawnPosition = MazeLayout::FRUIT_SPAWN);

    /// @brief Must be called every time Pac-Man eats a dot or power pellet.
    /// @param dotsEaten Dots eaten so far in this round, counting the one
    ///        just eaten.
    /// @param round The current round, which decides the fruit.
    void onDotEaten(int dotsEaten, int round);

    /// @brief Counts down the time of the active fruit.
    /// @param deltaTime Seconds since the last frame.
    void update(float deltaTime);

    /// @brief Lets Pac-Man eat the fruit if he is close enough.
    /// @param pacmanPosition Where Pac-Man is, in tiles.
    /// @return The fruit that was eaten (its points are scoreFor(type)), or
    ///         std::nullopt if there is none or Pac-Man is too far.
    [[nodiscard]] std::optional<Fruit> tryCollect(Position pacmanPosition);

    /// @brief The fruit currently on screen, if any.
    [[nodiscard]] const std::optional<Fruit> &getActiveFruit() const;

    /// @brief Removes the active fruit (new round or Pac-Man died).
    void resetRound();
};

#endif
