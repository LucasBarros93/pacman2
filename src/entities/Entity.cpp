#include "entities/Entity.hpp"

Entity::Entity(Position position, Direction direction, float speed)
    : position{position}, direction{direction}, speed{speed} {}

Position Entity::getPosition() const { return position; }

void Entity::setDirection(Direction newDirection) {
    if (newDirection != Direction::Neutral)
        this->direction = newDirection;
}
