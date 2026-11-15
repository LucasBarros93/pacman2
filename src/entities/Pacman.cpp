#include "entities/Pacman.hpp"
#include "maze/Maze.hpp"

Pacman::Pacman(Position spawnPosition)
    : Entity(spawnPosition, Direction::Neutral, DEFAULT_SPEED) {}

bool Pacman::trySetDirection(Direction newDirection, const Maze &maze) {
    if (!maze.canStep(getCurrentGridPosition(), newDirection))
        return false;

    direction = newDirection;
    return true;
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
        return; // eating: stays still and keeps the last value of "moving"
    }

    const Position before = position;
    Entity::update(maze, deltaTime);

    moving = position.x != before.x || position.y != before.y;
}
