#ifndef CORE_HPP
#define CORE_HPP

/// @file Core.hpp
/// @brief Basic types and helpers shared by the whole game.
///
/// Conventions used everywhere in the project:
/// - Positions are measured in tiles, not in pixels.
/// - The center of tile (x, y) is exactly at (x, y). A position with a
///   fractional part (like x = 13.5) lies between two tiles.
/// - The origin is the top-left corner of the maze and the y axis grows
///   downwards.
/// - Time is measured in seconds and speeds in tiles per second.

/// @brief A continuous position, in tiles.
struct Position {
    float x; ///< Horizontal position (grows to the right).
    float y; ///< Vertical position (grows downwards).
};

/// @brief The coordinates of a tile in the maze grid.
struct GridPosition {
    int x; ///< Column.
    int y; ///< Row.
};

/// @brief Compares two tiles.
[[nodiscard]] constexpr bool operator==(GridPosition a, GridPosition b) {
    return a.x == b.x && a.y == b.y;
}

/// @brief Compares two tiles.
[[nodiscard]] constexpr bool operator!=(GridPosition a, GridPosition b) {
    return !(a == b);
}

/// @brief A movement direction.
enum class Direction {
    Up,
    Down,
    Left,
    Right,
    Neutral ///< Not moving. It is not a real direction and has no opposite.
};

/// @brief Returns the direction that points the other way.
/// @param direction Any direction.
/// @return The opposite direction, or Direction::Neutral for
///         Direction::Neutral.
[[nodiscard]] constexpr Direction opposite(Direction direction) {
    switch (direction) {
    case Direction::Up:
        return Direction::Down;
    case Direction::Down:
        return Direction::Up;
    case Direction::Left:
        return Direction::Right;
    case Direction::Right:
        return Direction::Left;
    case Direction::Neutral:
        break;
    }
    return Direction::Neutral;
}

/// @brief Returns the tile next to another one, in a direction.
/// @param tile The starting tile.
/// @param direction Which side to go to.
/// @return The adjacent tile, or @p tile itself for Direction::Neutral.
/// @note It does not check the maze limits nor the tunnel (see Maze::wrap()).
[[nodiscard]] constexpr GridPosition neighbor(GridPosition tile,
                                              Direction direction) {
    switch (direction) {
    case Direction::Up:
        return {tile.x, tile.y - 1};
    case Direction::Down:
        return {tile.x, tile.y + 1};
    case Direction::Left:
        return {tile.x - 1, tile.y};
    case Direction::Right:
        return {tile.x + 1, tile.y};
    case Direction::Neutral:
        break;
    }
    return tile;
}

/// @brief The four ghosts.
/// @warning SpriteData uses the numeric value as the ghost's row in the sprite
///          sheet (guarded by a static_assert), so the order must match it.
enum class GhostType { Blinky, Pinky, Inky, Clyde };

/// @brief What the ghosts try to do while they walk in the maze.
///
/// Being frightened is separate: it works on top of the mode.
enum class GhostMode {
    Scatter, ///< Each ghost heads to its own corner of the maze.
    Chase    ///< Each ghost hunts Pac-Man in its own way.
};

/// @brief What a maze tile contains.
enum class CellType {
    Wall,        ///< Blocks every entity.
    Empty,       ///< Walkable, with nothing to collect.
    Pellet,      ///< Small dot.
    PowerPellet, ///< Big dot: frightens the ghosts.
};

/// @brief Collision radii and the overlap test. Everything is in tiles.
namespace Collision {
constexpr float PELLET_RADIUS =
    0.4f; ///< How close Pac-Man must be to eat a pellet.
constexpr float FRUIT_RADIUS =
    0.6f; ///< How close Pac-Man must be to eat a fruit.
constexpr float GHOST_RADIUS =
    0.5f; ///< How close Pac-Man and a ghost must be to touch.

/// @brief Tells whether two points are close enough to touch.
/// @param a First point.
/// @param b Second point.
/// @param radius Maximum distance between them, in tiles.
/// @return true if the distance between @p a and @p b is at most @p radius.
[[nodiscard]] constexpr bool overlaps(Position a, Position b, float radius) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return dx * dx + dy * dy <= radius * radius;
}
} // namespace Collision

#endif
