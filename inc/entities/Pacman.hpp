#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "core/Core.hpp"
#include "entities/Entity.hpp"

class Maze;

class Pacman : public Entity {
  private:
    bool canMove(const Maze &maze, Direction direction) const;

  public:
    explicit Pacman(const Position position);
    bool trySetDirection(Direction newDirection, const Maze &maze);
    void reset();
    void stop();
};

#endif
