#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Core.hpp"
#include "maze/Maze.hpp"

class Entity {
  protected:
    Position position;
    Direction direction;
    float speed;
    void setDirection(Direction newDirection);

  public:
    Entity(Position position, Direction direction, float speed);

    Position getPosition() const;
    Direction getDirection() const;

    void update(const Maze &maze, float deltaTime);
};

#endif
