#include "entities/Blinky.hpp"

Blinky::Blinky(Position spawnPosition)
    : Ghost(GhostType::Blinky, spawnPosition, Direction::Left,
            GhostState::Roaming) {}
