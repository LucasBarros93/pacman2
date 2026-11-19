#include "game/GhostHouse.hpp"
#include "entities/Ghost.hpp"

#include <algorithm>
#include <array>

namespace {
// Seconds of each phase, in order: scatter, chase, scatter, chase... After the
// last one the ghosts chase forever. The 1/60 s scatter of the later rounds is
// a single frame: it only exists to make the ghosts turn around once more.
constexpr std::array<float, 7> ROUND_1{7.0f, 20.0f, 7.0f, 20.0f,
                                       5.0f, 20.0f, 5.0f};
constexpr std::array<float, 7> ROUNDS_2_TO_4{7.0f, 20.0f,   7.0f,        20.0f,
                                             5.0f, 1033.0f, 1.0f / 60.0f};
constexpr std::array<float, 7> ROUNDS_5_PLUS{5.0f, 20.0f,   5.0f,        20.0f,
                                             5.0f, 1037.0f, 1.0f / 60.0f};

const std::array<float, 7> &phasesFor(int round) {
    if (round == 1)
        return ROUND_1;
    if (round <= 4)
        return ROUNDS_2_TO_4;
    return ROUNDS_5_PLUS;
}

/// Seconds between one ghost leaving the house and the next. It shrinks as
/// the rounds go by.
float releaseInterval(int round) {
    if (round == 1)
        return 4.0f;
    if (round < 4)
        return 3.0f;
    return 2.0f;
}
} // namespace

void GhostHouse::reset(int newRound) {
    round = newRound;
    phase = 0;
    modeTimer = 0.0f;
    releaseTimer =
        releaseInterval(newRound); // the first ghost leaves right away
}

bool GhostHouse::advanceMode(float deltaTime) {
    const auto &phases = phasesFor(round);

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
    if (releaseTimer < releaseInterval(round))
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
