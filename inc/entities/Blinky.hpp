#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "entities/Ghost.hpp"

class Blinky : public Ghost {
  public:
    explicit Blinky(Position spawnPosition);
};

#endif
