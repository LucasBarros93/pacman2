#ifndef SCOREPOPUP_HPP
#define SCOREPOPUP_HPP

#include "core/Core.hpp"

/// @brief A score shown for a short time where something was eaten.
///
/// Created by Game, counted down on every update and drawn by Renderer.
struct ScorePopup {
    Position position; ///< Where it is drawn, in tiles.
    int points;        ///< The value shown (picks the sprite).
    float remaining;   ///< Seconds until it disappears.
};

#endif
