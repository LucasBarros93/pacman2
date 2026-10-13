#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "entities/Entity.hpp"

class Pacman : public Entity {
  private:
    bool canMove(const Maze &maze, Direction direction) const;

  public:
    bool trySetDirection(Direction newDirection, const Maze &maze);
    Pacman(const Position position);
};

#endif
