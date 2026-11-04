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
    if (direction == Direction::Neutral)
        return false;

    GridPosition targetGridPosition =
        getGridPosition(position, Direction::Neutral);

    switch (direction) {
    case Direction::Up:
        --targetGridPosition.y;
        break;

    case Direction::Down:
        ++targetGridPosition.y;
        break;

    case Direction::Left:
        --targetGridPosition.x;
        break;

    case Direction::Right:
        ++targetGridPosition.x;
        break;

    case Direction::Neutral:
        return false;
    }

    targetGridPosition = teleportHorizontal(maze, targetGridPosition);

    return maze.isWalkable(targetGridPosition);
}

void Pacman::reset() {
    position = spawnPosition;
    direction = Direction::Neutral;
}

void Pacman::stop() { direction = Direction::Neutral; }
