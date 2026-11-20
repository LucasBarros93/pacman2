#include "entities/Inky.hpp"
#include "entities/Pacman.hpp"

namespace {
constexpr int TILES_AHEAD = 2;
} // namespace

Inky::Inky(const Pacman &pacman, const Ghost &blinky, Position spawnPosition)
    : Ghost(GhostType::Inky, spawnPosition), pacman{pacman}, blinky{blinky} {}

Direction Inky::chooseDirection(const Maze &,
                                const std::vector<Direction> &options) {
    if (isScattering())
        return chooseTowards(options, MazeLayout::INKY_SCATTER_TARGET);

    // the tile 2 ahead of Pac-Man
    const GridPosition step = neighbor({0, 0}, pacman.getDirection());
    const GridPosition pacmanTile = pacman.getCurrentGridPosition();
    const GridPosition pivot{pacmanTile.x + TILES_AHEAD * step.x,
                             pacmanTile.y + TILES_AHEAD * step.y};

    // the line from Blinky to the pivot, doubled
    const GridPosition blinkyTile = blinky.getCurrentGridPosition();
    const GridPosition target{2 * pivot.x - blinkyTile.x,
                              2 * pivot.y - blinkyTile.y};

    return chooseTowards(options, target);
}
