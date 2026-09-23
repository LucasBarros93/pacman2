#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "entities/Entity.hpp"

class Pacman : public Entity {
  public:
    Position nextPosition();
};

#endif
