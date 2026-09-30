#include "entities/Pacman.hpp"

Pacman::Pacman() : Entity({400.0f, 300.0f}, Direction::Neutral, 250.0f) {}

void Pacman::update(const Maze &maze, float deltaTime) {
    Position nextPosition;

    switch (this->direction) {

    case Direction::Up:
        nextPosition = this->position;
        nextPosition.second -= speed * deltaTime;

        if (maze.isWalkable(nextPosition))
            this->position = nextPosition;

        break;

    case Direction::Down:
        nextPosition = this->position;
        nextPosition.second += speed * deltaTime;

        if (maze.isWalkable(nextPosition))
            this->position = nextPosition;

        break;

    case Direction::Left:
        nextPosition = this->position;
        nextPosition.first -= speed * deltaTime;

        if (maze.isWalkable(nextPosition))
            this->position = nextPosition;

        break;

    case Direction::Right:
        nextPosition = this->position;
        nextPosition.first += speed * deltaTime;

        if (maze.isWalkable(nextPosition))
            this->position = nextPosition;

        break;

    case Direction::Neutral:
        break;
    }
}
