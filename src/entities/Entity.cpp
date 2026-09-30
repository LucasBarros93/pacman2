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
