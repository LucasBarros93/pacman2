#include "entities/Entity.hpp"
#include "core/Core.hpp"

Entity::Entity(Position position, Direction direction, float speed)
    : position{position}, direction{direction}, speed{speed} {}

Position Entity::getPosition() const { return position; }

Direction Entity::getDirection() const { return direction; }

void Entity::setDirection(Direction newDirection) {
    if (newDirection != Direction::Neutral)
        this->direction = newDirection;
}

GridPosition Entity::getGridPosition(Position position,
                                     Direction movementDirection) const {
    switch (movementDirection) {
    case Direction::Up:
        return {std::round(position.first), std::floor(position.second)};

    case Direction::Down:
        return {std::round(position.first), std::ceil(position.second)};

    case Direction::Left:
        return {std::floor(position.first), std::round(position.second)};

    case Direction::Right:
        return {std::ceil(position.first), std::round(position.second)};

    case Direction::Neutral:
        return {std::round(position.first), std::round(position.second)};
    }

    return {0, 0};
}

GridPosition Entity::teleportHorizontal(const Maze &maze,
                                        GridPosition gridPosition) const {
    if (gridPosition.first >= maze.getWidth()) {
        gridPosition.first = 0;
    } else if (gridPosition.first < 0) {
        gridPosition.first = maze.getWidth() - 1;
    }

    return gridPosition;
}

void Entity::update(const Maze &maze, float deltaTime) {
    Position nextPosition = position;

    switch (direction) {
    case Direction::Up:
        nextPosition.second -= speed * deltaTime;
        nextPosition.first = std::round(nextPosition.first);
        break;

    case Direction::Down:
        nextPosition.second += speed * deltaTime;
        nextPosition.first = std::round(nextPosition.first);
        break;

    case Direction::Left:
        nextPosition.first -= speed * deltaTime;
        nextPosition.second = std::round(nextPosition.second);
        break;

    case Direction::Right:
        nextPosition.first += speed * deltaTime;
        nextPosition.second = std::round(nextPosition.second);
        break;

    case Direction::Neutral:
        return;
    }

    const GridPosition nextGridPosition =
        getGridPosition(nextPosition, direction);

    const GridPosition teleportPosition =
        teleportHorizontal(maze, nextGridPosition);

    if (teleportPosition.first != nextGridPosition.first) {
        nextPosition.first =
            nextGridPosition.first < 0 ? maze.getWidth() - 1 : 0.0f;
    }

    if (maze.isWalkable(teleportPosition))
        position = nextPosition;
}
