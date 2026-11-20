#ifndef CLYDE_HPP
#define CLYDE_HPP

#include "entities/Ghost.hpp"
#include "maze/MazeLayout.hpp"

class Pacman;

/// @brief The orange ghost: chases Pac-Man from afar, but gets shy up close.
///
/// - Chase: while he is 8 or more tiles away from Pac-Man, his target is
///   Pac-Man's tile. When he gets closer than that, he heads to his corner.
/// - Scatter: the target is the bottom-left corner.
class Clyde : public Ghost {
  private:
    const Pacman &pacman; ///< The player.

  protected:
    Direction chooseDirection(const Maze &maze,
                              const std::vector<Direction> &options) override;

  public:
    /// @param pacman The player. It must outlive the ghost.
    /// @param spawnPosition Where he waits, in tiles.
    explicit Clyde(const Pacman &pacman,
                   Position spawnPosition = MazeLayout::CLYDE_SPAWN);
};

#endif
