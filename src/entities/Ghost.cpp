#include "entities/Ghost.hpp"
#include "maze/Maze.hpp"

#include <array>
#include <cmath>

namespace {
GridPosition neighbor(GridPosition tile, Direction direction) {
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

bool crossedTileCenter(Position before, Position after, Direction moved) {
    switch (moved) {
    case Direction::Right:
        return std::floor(after.x) != std::floor(before.x);
    case Direction::Left:
        return std::ceil(after.x) != std::ceil(before.x);
    case Direction::Down:
        return std::floor(after.y) != std::floor(before.y);
    case Direction::Up:
        return std::ceil(after.y) != std::ceil(before.y);
    case Direction::Neutral:
        break;
    }
    return false;
}
} // namespace

Ghost::Ghost(GhostType type, Position spawnPosition, Direction initialDirection)
    : Entity(spawnPosition, initialDirection, DEFAULT_SPEED), type{type} {}

GhostType Ghost::getType() const { return type; }

bool Ghost::canEnter(const Maze &maze, GridPosition tile) const {
    return maze.isGhostWalkable(tile);
}

Direction Ghost::chooseDirection(const Maze &, const std::vector<Direction> &) {
    return Direction::Neutral;
}

std::vector<Direction> Ghost::getAvailableDirections(const Maze &maze) const {
    static constexpr std::array<Direction, 4> ALL{
        Direction::Up, Direction::Down, Direction::Left, Direction::Right};

    const GridPosition tile = getCurrentGridPosition();
    std::vector<Direction> options;

    for (const Direction candidate : ALL) {
        if (candidate == opposite(direction))
            continue;

        const GridPosition target =
            teleportHorizontal(maze, neighbor(tile, candidate));

        if (!canEnter(maze, target))
            continue;

        if (candidate == Direction::Down &&
            maze.getCell(target).cellType == CellType::GhostDoor)
            continue;

        options.push_back(candidate);
    }

    if (options.empty())
        options.push_back(opposite(direction));

    return options;
}

void Ghost::update(const Maze &maze, float deltaTime) {
    if (direction == Direction::Neutral)
        return;
    const Position before = position;
    const Direction moved = direction;

    Entity::update(maze, deltaTime);

    const bool blocked = position.x == before.x && position.y == before.y;

    if (!blocked && !crossedTileCenter(before, position, moved))
        return;

    const Direction next = chooseDirection(maze, getAvailableDirections(maze));

    if (blocked || next != direction)
        position = {std::round(position.x), std::round(position.y)};

    direction = next;
}
