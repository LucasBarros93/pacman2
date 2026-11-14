#include "entities/Pacman.hpp"
#include "maze/Maze.hpp"

Pacman::Pacman(Position position)
    : Entity({position}, Direction::Neutral, DEFAULT_SPEED) {}

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

void Pacman::stop() {
    direction = Direction::Neutral;
    moving = false;
}

bool Pacman::isMoving() const { return moving; }

void Pacman::stall(float duration) { stallTime = duration; }

void Pacman::reset() {
    Entity::reset();
    stallTime = 0.0f;
    moving = false;
}

void Pacman::update(const Maze &maze, float deltaTime) {
    if (stallTime > 0.0f) {
        stallTime -= deltaTime;
        return;
    }

    const Position before = position;
    Entity::update(maze, deltaTime);

    moving = position.x != before.x || position.y != before.y;
}
