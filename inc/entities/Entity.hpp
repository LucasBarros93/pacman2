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
    virtual void update(const Maze &maze);
};

#endif
