#ifndef INKY_HPP
#define INKY_HPP

#include "entities/Ghost.hpp"
#include "maze/MazeLayout.hpp"

class Pacman;

/// @brief The cyan ghost: his target depends on Pac-Man and on Blinky.
///
/// - Chase: take the tile 2 ahead of Pac-Man (in the direction he faces),
///   draw a line from Blinky to it and double it. The end of the line is the
///   target. The arcade bug of "facing up" is not reproduced.
/// - Scatter: the target is the bottom-right corner.
class Inky : public Ghost {
  private:
    const Pacman &pacman; ///< The player.
    const Ghost &blinky;  ///< The ghost he uses as a reference.

  protected:
    Direction chooseDirection(const Maze &maze,
                              const std::vector<Direction> &options) override;

  public:
    /// @param pacman The player. It must outlive the ghost.
    /// @param blinky Blinky. It must outlive the ghost.
    /// @param spawnPosition Where he waits, in tiles.
    Inky(const Pacman &pacman, const Ghost &blinky,
         Position spawnPosition = MazeLayout::INKY_SPAWN);
};

#endif
