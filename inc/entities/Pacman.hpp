#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "entities/Entity.hpp"

class Pacman : public Entity {
  public:
    Pacman();
    void update(const Maze &maze, float deltaTime) override;
};

#endif
