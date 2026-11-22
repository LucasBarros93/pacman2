#include "game/GhostHouse.hpp"
#include "core/Difficulty.hpp"
#include "entities/Ghost.hpp"

#include <algorithm>
#include <array>

void GhostHouse::reset(int newRound) {
    round = newRound;
    phase = 0;
    modeTimer = 0.0f;
    releaseTimer = Difficulty::ghostReleaseInterval(
        newRound); // the first ghost leaves right away
}

bool GhostHouse::advanceMode(float deltaTime) {
    const auto &phases = Difficulty::modePhases(round);

    if (phase >= phases.size())
        return false; // the last chase never ends

    modeTimer += deltaTime;
    if (modeTimer < phases[phase])
        return false;

    modeTimer -= phases[phase];
    ++phase;
    return true;
}

void GhostHouse::update(float deltaTime,
                        const std::vector<std::unique_ptr<Ghost>> &ghosts) {
    // the mode clock stops while a ghost is frightened
    const bool frightened =
        std::any_of(ghosts.begin(), ghosts.end(),
                    [](const auto &ghost) { return ghost->isFrightened(); });

    if (!frightened && advanceMode(deltaTime))
        for (const auto &ghost : ghosts)
            ghost->setMode(getMode());

    // release: one ghost at a time, in the order of the vector
    releaseTimer += deltaTime;
    if (releaseTimer < Difficulty::ghostReleaseInterval(round))
        return;

    for (const auto &ghost : ghosts) {
        if (!ghost->isInHouse())
            continue;

        ghost->leaveHouse();

        for (const auto &other : ghosts)
            other->onGhostReleased(ghost->getType());

        releaseTimer = 0.0f;
        return;
    }
}

GhostMode GhostHouse::getMode() const {
    // even phases are scatter, odd ones are chase (and so is everything after
    // the last phase)
    return phase % 2 == 0 ? GhostMode::Scatter : GhostMode::Chase;
}
