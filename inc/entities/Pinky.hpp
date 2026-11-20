#ifndef PINKY_HPP
#define PINKY_HPP

#include "entities/Ghost.hpp"
#include "maze/MazeLayout.hpp"

class Pacman;

/// @brief The pink ghost: aims at the tiles in front of Pac-Man.
///
/// - Chase: the target is 4 tiles ahead of Pac-Man, in the direction he is
///   facing. Like in the arcade, when he faces up the target is also 4 tiles
///   to the left (a bug of the original game that became part of the ghost).
/// - Scatter: the target is the top-left corner.
class Pinky : public Ghost {
  private:
    const Pacman &pacman; ///< The player he aims ahead of.

  protected:
    Direction chooseDirection(const Maze &maze,
                              const std::vector<Direction> &options) override;

  public:
    /// @param pacman The player. It must outlive the ghost.
    /// @param spawnPosition Where he waits, in tiles.
    explicit Pinky(const Pacman &pacman,
                   Position spawnPosition = MazeLayout::PINKY_SPAWN);
};

#endif
