#include "entities/Entity.hpp"

Entity::Entity(Position position, Direction direction, float speed) {
    this->position = position;
    this->direction = direction;
    this->speed = speed;
}

Position Entity::getPosition() const { return position; }

void Entity::setDirection(Direction newDirection) {
    if (newDirection != Direction::Neutral)
        this->direction = newDirection;
}
