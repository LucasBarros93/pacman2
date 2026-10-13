#include "entities/Pacman.hpp"

Pacman::Pacman(Position position)
    : Entity({position}, Direction::Neutral, 6.5f) {}

bool Pacman::trySetDirection(Direction newDirection, const Maze &maze) {
    if (!canMove(maze, newDirection))
        return false;

    this->setDirection(newDirection);
    return true;
}

bool Pacman::canMove(const Maze &maze, Direction direction) const {
    Position nextPosition = this->position;
    GridPosition nextGridPosition;

    switch (direction) {
    case Direction::Up:
        nextPosition.second -= speed * 0.016f;
        nextPosition.first = std::round(nextPosition.first);

        nextGridPosition = {static_cast<int>(std::round(nextPosition.first)),
                            static_cast<int>(std::floor(nextPosition.second))};
        break;

    case Direction::Down:
        nextPosition.second += speed * 0.016f;
        nextPosition.first = std::round(nextPosition.first);

        nextGridPosition = {static_cast<int>(std::round(nextPosition.first)),
                            static_cast<int>(std::ceil(nextPosition.second))};
        break;

    case Direction::Left:
        nextPosition.first -= speed * 0.016f;
        nextPosition.second = std::round(nextPosition.second);

        nextGridPosition = {static_cast<int>(std::floor(nextPosition.first)),
                            static_cast<int>(std::round(nextPosition.second))};
        break;

    case Direction::Right:
        nextPosition.first += speed * 0.016f;
        nextPosition.second = std::round(nextPosition.second);

        nextGridPosition = {static_cast<int>(std::ceil(nextPosition.first)),
                            static_cast<int>(std::round(nextPosition.second))};
        break;

    case Direction::Neutral:
        return false;
    }

    // Wrap horizontal, como no Entity::update().
    if (nextGridPosition.first >= maze.getWidth())
        nextGridPosition.first = 0;

    if (nextGridPosition.first < 0)
        nextGridPosition.first = maze.getWidth() - 1;

    return maze.isWalkable(nextGridPosition);
}
