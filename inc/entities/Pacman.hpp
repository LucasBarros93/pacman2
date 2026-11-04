#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "entities/Entity.hpp"

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
