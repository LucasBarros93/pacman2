#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "entities/Ghost.hpp"
#include "maze/MazeLayout.hpp"

class Maze;
class Pacman;

/// @brief The red ghost: starts outside the house, already walking left.
///
/// - In chase mode his target is Pac-Man's tile.
/// - In scatter mode his target is the top-right corner.
/// - Cruise Elroy: when few pellets are left he gets faster and keeps chasing
///   Pac-Man even in scatter mode. The number of pellets that triggers it
///   shrinks as the rounds go by. After Pac-Man dies, it only comes back when
///   Clyde leaves the house.
class Blinky : public Ghost {
  private:
    static constexpr float ELROY_1_FACTOR =
        80.0f / 75.0f; ///< Speed of Elroy 1, relative to a normal ghost.
    static constexpr float ELROY_2_FACTOR =
        85.0f / 75.0f; ///< Speed of Elroy 2, relative to a normal ghost.

    const Pacman &pacman; ///< The player he hunts.
    bool elroySuspended =
        false; ///< Elroy is off after Pac-Man dies, until Clyde leaves.

    /// @brief 0 if he is normal, 1 or 2 for the two stages of Cruise Elroy.
    [[nodiscard]] int getElroyLevel(const Maze &maze) const;

  protected:
    Direction chooseDirection(const Maze &maze,
                              const std::vector<Direction> &options) override;

    float runSpeed(const Maze &maze) const override;

  public:
    /// @param pacman The player he hunts. It must outlive the ghost.
    /// @param spawnPosition Where he starts, in tiles.
    explicit Blinky(const Pacman &pacman,
                    Position spawnPosition = MazeLayout::BLINKY_SPAWN);

    void reset() override;
    void onPacmanDied() override;
    void onGhostReleased(GhostType releasedType) override;
};

#endif
