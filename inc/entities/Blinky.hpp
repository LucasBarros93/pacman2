#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "entities/Ghost.hpp"
#include "maze/MazeLayout.hpp"

/// @brief The red ghost: starts outside the house, already walking left.
///
/// For now it behaves like any other ghost. It is a class of its own so that
/// its AI can be added later by overriding Ghost::chooseDirection().
class Blinky : public Ghost {
  public:
    /// @param spawnPosition Where it starts, in tiles.
    explicit Blinky(Position spawnPosition = MazeLayout::BLINKY_SPAWN);
};

#endif
