#include "entities/Clyde.hpp"
#include "entities/Pacman.hpp"

namespace {
constexpr int SHY_DISTANCE =
    8; ///< Tiles. Closer than this, he runs to his corner.
} // namespace

Clyde::Clyde(const Pacman &pacman, Position spawnPosition)
    : Ghost(GhostType::Clyde, spawnPosition), pacman{pacman} {}

Direction Clyde::chooseDirection(const Maze &,
                                 const std::vector<Direction> &options) {
    const GridPosition pacmanTile = pacman.getCurrentGridPosition();
    const GridPosition myTile = getCurrentGridPosition();

    const int dx = pacmanTile.x - myTile.x;
    const int dy = pacmanTile.y - myTile.y;
    const bool tooClose = dx * dx + dy * dy < SHY_DISTANCE * SHY_DISTANCE;

    // in scatter mode, or when too close, he goes to his corner
    const GridPosition target = (isScattering() || tooClose)
                                    ? MazeLayout::CLYDE_SCATTER_TARGET
                                    : pacmanTile;

    return chooseTowards(options, target);
}
