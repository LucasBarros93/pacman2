#include "entities/Pinky.hpp"
#include "entities/Pacman.hpp"

namespace {
constexpr int TILES_AHEAD = 4;
} // namespace

Pinky::Pinky(const Pacman &pacman, Position spawnPosition)
    : Ghost(GhostType::Pinky, spawnPosition), pacman{pacman} {}

Direction Pinky::chooseDirection(const Maze &,
                                 const std::vector<Direction> &options) {
    if (isScattering())
        return chooseTowards(options, MazeLayout::PINKY_SCATTER_TARGET);

    const Direction facing = pacman.getDirection();
    const GridPosition step = neighbor({0, 0}, facing); // one tile that way

    GridPosition target = pacman.getCurrentGridPosition();
    target.x += TILES_AHEAD * step.x;
    target.y += TILES_AHEAD * step.y;

    if (facing == Direction::Up)
        target.x -= TILES_AHEAD; // arcade bug, kept on purpose

    return chooseTowards(options, target);
}
