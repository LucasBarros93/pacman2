#include "entities/Blinky.hpp"
#include "core/Difficulty.hpp"
#include "entities/Pacman.hpp"
#include "maze/Maze.hpp"

Blinky::Blinky(const Pacman &pacman, Position spawnPosition)
    : Ghost(GhostType::Blinky, spawnPosition, Direction::Right,
            GhostState::Scatter),
      pacman{pacman} {}

void Blinky::reset() {
    Ghost::reset();
    elroySuspended = false;
}

void Blinky::onPacmanDied() { elroySuspended = true; }

void Blinky::onGhostReleased(GhostType releasedType) {
    if (releasedType == GhostType::Clyde)
        elroySuspended = false;
}

int Blinky::getElroyLevel(const Maze &maze) const {
    if (elroySuspended)
        return 0;

    const int threshold = Difficulty::elroyDotsLeft(getRound());
    const int dotsLeft = maze.getRemainingCollectibles();

    if (dotsLeft <= threshold / 2)
        return 2;
    if (dotsLeft <= threshold)
        return 1;
    return 0;
}

float Blinky::runSpeed(const Maze &maze) const {
    const float base = Ghost::runSpeed(maze);

    switch (getElroyLevel(maze)) {
    case 1:
        return base * ELROY_1_FACTOR;
    case 2:
        return base * ELROY_2_FACTOR;
    default:
        return base;
    }
}

Direction Blinky::chooseDirection(const Maze &maze,
                                  const std::vector<Direction> &options) {
    // In scatter mode he goes to his corner, unless he is a Cruise Elroy:
    // then he keeps chasing.
    const bool goToCorner = isScattering() && getElroyLevel(maze) == 0;

    const GridPosition target = goToCorner ? MazeLayout::BLINKY_SCATTER_TARGET
                                           : pacman.getCurrentGridPosition();

    return chooseTowards(options, target);
}
