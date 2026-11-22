#ifndef DIFFICULTY_HPP
#define DIFFICULTY_HPP

#include <array>

/// @file Difficulty.hpp
/// @brief How the game gets harder as the rounds go by.
///
/// The multipliers are relative to the first round and follow the proportions
/// of the original game, rounded to simple values.
namespace Difficulty {

/// @brief Multiplier of Pac-Man's speed.
/// @param round The round number, starting at 1.
[[nodiscard]] constexpr float pacmanSpeedScale(int round) {
    if (round == 1)
        return 1.0f;
    if (round <= 4)
        return 1.125f;
    if (round <= 20)
        return 1.25f;
    return 1.125f; // from round 21 Pac-Man slows down again, the ghosts do not
}

/// @brief Multiplier of the speed of the ghosts (not of the eyes).
/// @param round The round number, starting at 1.
[[nodiscard]] constexpr float ghostSpeedScale(int round) {
    if (round == 1)
        return 1.0f;
    if (round <= 4)
        return 1.13f;
    return 1.26f;
}

/// @brief Seconds a power pellet frightens the ghosts. It shrinks as the
///        rounds go by and ends up at zero (the ghosts only turn around).
/// @param round The round number, starting at 1.
[[nodiscard]] constexpr float frightenedDuration(int round) {
    if (round <= 5)
        return 7.0f - static_cast<float>(round); // 6, 5, 4, 3, 2
    if (round <= 8)
        return 2.0f;
    if (round <= 18)
        return 1.0f;
    return 0.0f;
}

/// @brief Seconds between one ghost leaving the house and the next.
/// @param round The round number, starting at 1.
[[nodiscard]] constexpr float ghostReleaseInterval(int round) {
    if (round == 1)
        return 4.0f;
    if (round < 4)
        return 3.0f;
    return 2.0f;
}

/// @brief Pellets left when Blinky becomes Cruise Elroy 1 (Elroy 2 starts at
///        half of it).
/// @param round The round number, starting at 1.
[[nodiscard]] constexpr int elroyDotsLeft(int round) {
    if (round == 1)
        return 20;
    if (round == 2)
        return 30;
    if (round <= 5)
        return 40;
    if (round <= 8)
        return 50;
    if (round <= 11)
        return 60;
    if (round <= 14)
        return 80;
    if (round <= 18)
        return 100;
    return 120;
}

// Seconds of each scatter/chase phase, in order: scatter, chase, scatter,
// chase... After the last one the ghosts chase forever. The 1/60 s scatter of
// the later rounds is a single frame: it only makes the ghosts turn around
// once more.
inline constexpr std::array<float, 7> MODE_PHASES_ROUND_1{
    7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 20.0f, 5.0f};
inline constexpr std::array<float, 7> MODE_PHASES_ROUNDS_2_TO_4{
    7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 1033.0f, 1.0f / 60.0f};
inline constexpr std::array<float, 7> MODE_PHASES_ROUNDS_5_PLUS{
    5.0f, 20.0f, 5.0f, 20.0f, 5.0f, 1037.0f, 1.0f / 60.0f};

/// @brief The durations of the scatter/chase phases of a round.
/// @param round The round number, starting at 1.
[[nodiscard]] constexpr const std::array<float, 7> &modePhases(int round) {
    if (round == 1)
        return MODE_PHASES_ROUND_1;
    if (round <= 4)
        return MODE_PHASES_ROUNDS_2_TO_4;
    return MODE_PHASES_ROUNDS_5_PLUS;
}

} // namespace Difficulty

#endif
