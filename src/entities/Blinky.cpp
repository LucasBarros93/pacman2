#include "entities/Blinky.hpp"

Blinky::Blinky(Position spawnPosition)
    : Ghost(GhostType::Blinky, spawnPosition) {}

Direction Blinky::chooseDirection(const Maze &,
                                  const std::vector<Direction> &options) {
    std::uniform_int_distribution<std::size_t> pick(0, options.size() - 1);
    return options[pick(rng)];
}
