#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "entities/Ghost.hpp"

#include <random>

class Blinky : public Ghost {
  private:
    std::mt19937 rng{std::random_device{}()};

  protected:
    Direction chooseDirection(const Maze &maze,
                              const std::vector<Direction> &options) override;

  public:
    explicit Blinky(Position spawnPosition);
};

#endif
