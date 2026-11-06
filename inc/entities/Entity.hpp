#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "core/Core.hpp"

class Maze;

class Entity {
  protected:
    Position position;
    Direction direction;
    float speed;
    Position spawnPosition;
    Direction spawnDirection;

    void setDirection(Direction newDirection);

    GridPosition getGridPosition(Position position,
                                 Direction movementDirection) const;

    GridPosition teleportHorizontal(const Maze &maze,
                                    GridPosition gridPosition) const;

    virtual bool canEnter(const Maze &maze, GridPosition tile) const;

  public:
    Entity(Position position, Direction direction, float speed);
    virtual ~Entity() = default;

    Position getPosition() const;
    Direction getDirection() const;
    GridPosition getCurrentGridPosition() const;

    virtual void update(const Maze &maze, float deltaTime);
    virtual void reset();
};

#endif
