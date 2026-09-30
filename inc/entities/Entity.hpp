#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Core.hpp"
#include "maze/Maze.hpp"

class Entity {
  protected:
    Position position;
    Direction direction;
    float speed;

  public:
    Entity(Position position, Direction direction, float speed);
    virtual void update(const Maze &maze, float deltaTime) = 0;
    Position getPosition() const;
    Direction getDirection() const;
    void setDirection(Direction newDirection);
};

#endif
