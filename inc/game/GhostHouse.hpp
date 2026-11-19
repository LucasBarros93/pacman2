#ifndef GHOSTHOUSE_HPP
#define GHOSTHOUSE_HPP

#include "core/Core.hpp"

#include <cstddef>
#include <memory>
#include <vector>

class Ghost;

/// @brief Decides when the ghosts leave the house and which mode they are in.
///
/// - Release: one ghost at a time, in the order of the vector given to
///   update(). The time between them shrinks as the rounds go by, and the
///   first one leaves right away.
/// - Mode: every round starts in scatter and alternates with chase following
///   a table that depends on the round. The mode clock stops while a ghost
///   is frightened, like in the original game.
class GhostHouse {
  private:
    int round = 1;             ///< Round that decides the durations.
    std::size_t phase = 0;     ///< Index of the current scatter/chase phase.
    float modeTimer = 0.0f;    ///< Seconds spent in the current phase.
    float releaseTimer = 0.0f; ///< Seconds since the last ghost left the house.

    /// @brief Advances the mode clock.
    /// @return true if the mode changed.
    bool advanceMode(float deltaTime);

    /// @brief The current mode.
    GhostMode getMode() const;

  public:
    /// @brief Starts over (new round, new game, or after dying): scatter
    ///        mode, and the first ghost ready to leave.
    /// @param newRound The round number, starting at 1.
    void reset(int newRound);

    /// @brief Lets the ghosts out when their time comes and switches their
    ///        mode when the clock says so.
    /// @param deltaTime Seconds since the last frame.
    /// @param ghosts The ghosts, in the order they leave the house.
    void update(float deltaTime,
                const std::vector<std::unique_ptr<Ghost>> &ghosts);
};

#endif
