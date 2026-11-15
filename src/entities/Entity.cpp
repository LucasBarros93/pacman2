#include "entities/Entity.hpp"
#include "maze/Maze.hpp"

#include <cmath>

namespace {
/// The tile reached when moving in a direction. Along the axis of movement it
/// rounds towards the front (so it is the tile being entered); along the
/// other axis, and for Direction::Neutral, it rounds to the nearest tile.
GridPosition tileAhead(Position from, Direction heading) {
    const auto toInt = [](float value) { return static_cast<int>(value); };

    switch (heading) {
    case Direction::Up:
        return {toInt(std::round(from.x)), toInt(std::floor(from.y))};
    case Direction::Down:
        return {toInt(std::round(from.x)), toInt(std::ceil(from.y))};
    case Direction::Left:
        return {toInt(std::floor(from.x)), toInt(std::round(from.y))};
    case Direction::Right:
        return {toInt(std::ceil(from.x)), toInt(std::round(from.y))};
    case Direction::Neutral:
        break;
    }
    return {toInt(std::round(from.x)), toInt(std::round(from.y))};
}
} // namespace

Entity::Entity(Position position, Direction direction, float speed)
    : position{position}, direction{direction}, speed{speed},
      spawnPosition{position}, spawnDirection{direction} {}

Position Entity::getPosition() const { return position; }

Direction Entity::getDirection() const { return direction; }

GridPosition Entity::getCurrentGridPosition() const {
    return tileAhead(position, Direction::Neutral);
}

void Entity::setVisible(bool value) { visible = value; }

bool Entity::isVisible() const { return visible; }

void Entity::update(const Maze &maze, float deltaTime) {
    Position next = position;

    switch (direction) {
    case Direction::Up:
        next.y -= speed * deltaTime;
        next.x = std::round(next.x);
        break;
    case Direction::Down:
        next.y += speed * deltaTime;
        next.x = std::round(next.x);
        break;
    case Direction::Left:
        next.x -= speed * deltaTime;
        next.y = std::round(next.y);
        break;
    case Direction::Right:
        next.x += speed * deltaTime;
        next.y = std::round(next.y);
        break;
    case Direction::Neutral:
        return;
    }

    const GridPosition ahead = tileAhead(next, direction);
    const GridPosition target = maze.wrap(ahead);

    // went through the tunnel: reappears at the other edge
    if (target.x != ahead.x)
        next.x = static_cast<float>(target.x);

    if (maze.isWalkable(target))
        position = next;
}

void Entity::reset() {
    position = spawnPosition;
    direction = spawnDirection;
    visible = true;
}
