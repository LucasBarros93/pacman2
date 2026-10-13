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

    GridPosition getGridPosition(Position position,
                                 Direction movementDirection) const;

    GridPosition teleportHorizontal(const Maze &maze,
                                    GridPosition gridPosition) const;

  public:
    Entity(Position position, Direction direction, float speed);
    virtual ~Entity() = default;

    Position getPosition() const;
    Direction getDirection() const;

    void update(const Maze &maze, float deltaTime);
};

#endif
