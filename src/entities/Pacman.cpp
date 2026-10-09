#include "entities/Pacman.hpp"

Pacman::Pacman(Position position)
    : Entity({position}, Direction::Neutral, 6.5f) {}

void Pacman::update(const Maze &maze, float deltaTime) {
    Position nextPosition = this->position;
    GridPosition nextGridPosition;

    switch (this->direction) {

    case Direction::Up:
        nextPosition.second -= speed * deltaTime;
        nextPosition.first = std::round(nextPosition.first);
        nextGridPosition = {std::round(nextPosition.first),
                            std::floor(nextPosition.second)};
        break;

    case Direction::Down:
        nextPosition.second += speed * deltaTime;
        nextPosition.first = std::round(nextPosition.first);
        nextGridPosition = {std::round(nextPosition.first),
                            std::ceil(nextPosition.second)};
        break;

    case Direction::Left:
        nextPosition.first -= speed * deltaTime;
        nextPosition.second = std::round(nextPosition.second);
        nextGridPosition = {std::floor(nextPosition.first),
                            std::round(nextPosition.second)};
        break;

    case Direction::Right:
        nextPosition.first += speed * deltaTime;
        nextPosition.second = std::round(nextPosition.second);
        nextGridPosition = {std::ceil(nextPosition.first),
                            std::round(nextPosition.second)};
        break;

    case Direction::Neutral:
        return;
    }

    if (nextGridPosition.first > maze.getWidth() - 1) {
        nextPosition.first = 0;
        nextGridPosition.first = 0;
    }

    if (nextGridPosition.first < 0) {
        nextPosition.first = maze.getWidth() - 1;
        nextGridPosition.first = maze.getWidth() - 1;
    }

    if (maze.isWalkable(nextGridPosition))
        this->position = nextPosition;
}
