#ifndef CORE_HPP
#define CORE_HPP

struct Position {
    float x, y;
};

struct GridPosition {
    int x, y;
};

enum class Direction { Up, Down, Left, Right, Neutral };

inline Direction opposite(Direction direction) {
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

enum class GhostType { Blinky, Pinky, Inky, Clyde };

enum class CellType { Wall, Empty, Pellet, PowerPellet, GhostDoor };

struct Cell {
    CellType cellType;
};

namespace Collision {
constexpr float PELLET_RADIUS = 0.4f;
constexpr float FRUIT_RADIUS = 0.6f;
constexpr float GHOST_RADIUS = 0.5f;

inline bool overlaps(Position a, Position b, float radius) {
    const float dx = a.x - b.x;
    const float dy = a.y - b.y;
    return dx * dx + dy * dy <= radius * radius;
}
} // namespace Collision

#endif
