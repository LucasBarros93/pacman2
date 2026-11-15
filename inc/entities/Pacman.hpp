#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "core/Core.hpp"
#include "entities/Entity.hpp"
#include "maze/MazeLayout.hpp"

class Maze;

/// @brief The player's character.
///
/// Moves in the direction the player chose and stops at walls. It also pauses
/// for a moment every time it eats a pellet.
class Pacman : public Entity {
  private:
    static constexpr float DEFAULT_SPEED = 6.5f; ///< Tiles per second.

    float stallTime = 0.0f; ///< Seconds left of the pause caused by eating.
    bool moving = false;    ///< Whether the last update moved Pac-Man.

  public:
    /// @param spawnPosition Where Pac-Man starts, in tiles.
    explicit Pacman(Position spawnPosition = MazeLayout::PACMAN_SPAWN);

    /// @brief Changes direction if the tile in that direction is walkable.
    /// @param newDirection The direction the player asked for.
    /// @param maze The maze Pac-Man is in.
    /// @return true if the direction was changed, false if a wall blocked it
    ///         (or if @p newDirection is Direction::Neutral).
    bool trySetDirection(Direction newDirection, const Maze &maze);

    /// @brief Stops Pac-Man (direction becomes Direction::Neutral).
    void stop();

    /// @brief Tells whether Pac-Man moved in the last update (used to animate
    ///        the mouth only while he walks).
    [[nodiscard]] bool isMoving() const;

    /// @brief Freezes Pac-Man for a short time, as when eating a pellet.
    /// @param duration Seconds to stay still.
    void stall(float duration);

    void update(const Maze &maze, float deltaTime) override;
    void reset() override;
};

#endif
