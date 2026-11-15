#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Core.hpp"

class Maze;

/// @brief Anything that moves through the maze (Pac-Man and the ghosts).
///
/// An entity moves in a straight line in its current direction and stops
/// when the tile ahead is a wall. Subclasses decide when to change direction.
/// The two sides of the maze are connected by the tunnel.
class Entity {
  protected:
    Position position;   ///< Current position, in tiles.
    Direction direction; ///< Direction it is moving in.
    float speed;         ///< Tiles per second.

  private:
    Position spawnPosition;   ///< Where reset() puts the entity back.
    Direction spawnDirection; ///< The direction it has after reset().
    bool visible = true;      ///< Hidden entities are not drawn.

  public:
    /// @param position Starting position, in tiles.
    /// @param direction Starting direction.
    /// @param speed Speed, in tiles per second.
    Entity(Position position, Direction direction, float speed);
    virtual ~Entity() = default;

    [[nodiscard]] Position getPosition() const;
    [[nodiscard]] Direction getDirection() const;

    /// @brief The tile the entity is closest to.
    [[nodiscard]] GridPosition getCurrentGridPosition() const;

    void setVisible(bool value);
    [[nodiscard]] bool isVisible() const;

    /// @brief Moves the entity for one frame.
    ///
    /// The step is not taken if it would enter a wall. Going out through a
    /// side of the maze brings the entity back through the other side. The
    /// axis perpendicular to the movement is rounded to the nearest tile
    /// center, so a position like x = 13.5 only survives while moving
    /// horizontally.
    /// @param maze The maze the entity walks in.
    /// @param deltaTime Seconds since the last frame.
    virtual void update(const Maze &maze, float deltaTime);

    /// @brief Puts the entity back at its starting position and direction,
    ///        and makes it visible.
    virtual void reset();
};

#endif
