#include "entities/Entity.hpp"
#include "core/Core.hpp"
#include "maze/Maze.hpp"

#include <cmath>

Entity::Entity(Position position, Direction direction, float speed)
    : position{position}, direction{direction}, speed{speed},
      spawnPosition{position} {}

Position Entity::getPosition() const { return position; }

Direction Entity::getDirection() const { return direction; }

GridPosition Entity::getCurrentGridPosition() const {
    return getGridPosition(position, Direction::Neutral);
}

void Entity::setDirection(Direction newDirection) {
    if (newDirection != Direction::Neutral)
        this->direction = newDirection;
}

GridPosition Entity::getGridPosition(Position position,
                                     Direction movementDirection) const {
    switch (movementDirection) {
    case Direction::Up:
        return {static_cast<int>(std::round(position.x)),
                static_cast<int>(std::floor(position.y))};

    case Direction::Down:
        return {static_cast<int>(std::round(position.x)),
                static_cast<int>(std::ceil(position.y))};

    case Direction::Left:
        return {static_cast<int>(std::floor(position.x)),
                static_cast<int>(std::round(position.y))};

    case Direction::Right:
        return {static_cast<int>(std::ceil(position.x)),
                static_cast<int>(std::round(position.y))};

    case Direction::Neutral:
        return {static_cast<int>(std::round(position.x)),
                static_cast<int>(std::round(position.y))};
    }

    return {0, 0};
}

GridPosition Entity::teleportHorizontal(const Maze &maze,
                                        GridPosition gridPosition) const {
    if (gridPosition.x >= maze.getWidth()) {
        gridPosition.x = 0;
    } else if (gridPosition.x < 0) {
        gridPosition.x = maze.getWidth() - 1;
    }

    return gridPosition;
}

void Entity::update(const Maze &maze, float deltaTime) {
    Position nextPosition = position;

    switch (direction) {
    case Direction::Up:
        nextPosition.y -= speed * deltaTime;
        nextPosition.x = std::round(nextPosition.x);
        break;

    case Direction::Down:
        nextPosition.y += speed * deltaTime;
        nextPosition.x = std::round(nextPosition.x);
        break;

    case Direction::Left:
        nextPosition.x -= speed * deltaTime;
        nextPosition.y = std::round(nextPosition.y);
        break;

    case Direction::Right:
        nextPosition.x += speed * deltaTime;
        nextPosition.y = std::round(nextPosition.y);
        break;

    case Direction::Neutral:
        return;
    }

    const GridPosition nextGridPosition =
        getGridPosition(nextPosition, direction);

    const GridPosition teleportPosition =
        teleportHorizontal(maze, nextGridPosition);

    if (teleportPosition.x != nextGridPosition.x) {
        nextPosition.x = nextGridPosition.x < 0 ? maze.getWidth() - 1 : 0.0f;
    }

    if (maze.isWalkable(teleportPosition))
        position = nextPosition;
}
