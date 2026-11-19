#ifndef MAZELAYOUT_HPP
#define MAZELAYOUT_HPP

#include "core/Core.hpp"

#include <array>

/// @file MazeLayout.hpp
/// @brief Fixed positions of assets/maps/map.txt, the only maze of the game.
///
/// Everything is in tiles (see Core.hpp) and depends on the exact layout of
/// the map file. If the map changes, this is the only file that has to change.
namespace MazeLayout {

/// @name Spawns
/// @{
constexpr Position PACMAN_SPAWN{13.5f, 23.0f}; ///< Bottom corridor, centered.
constexpr Position FRUIT_SPAWN{
    13.5f, 17.0f}; ///< Corridor right below the ghost house.
/// @}

/// @name Ghost house
/// The house is a closed box (columns 11 to 16, rows 13 to 15). Ghosts leave
/// it following a route: first to HOUSE_CENTER_X, then up to HOUSE_EXIT_Y.
/// @{
constexpr float HOUSE_CENTER_X = 13.5f; ///< Horizontal center of the house.
constexpr float HOUSE_CENTER_Y = 14.0f; ///< Row where the ghosts wait inside.
constexpr float HOUSE_EXIT_Y = 11.0f;   ///< Corridor right above the house.

constexpr Position BLINKY_SPAWN{HOUSE_CENTER_X, HOUSE_EXIT_Y};
constexpr Position PINKY_SPAWN{HOUSE_CENTER_X, HOUSE_CENTER_Y};
constexpr Position INKY_SPAWN{HOUSE_CENTER_X - 2.0f, HOUSE_CENTER_Y};
constexpr Position CLYDE_SPAWN{HOUSE_CENTER_X + 2.0f, HOUSE_CENTER_Y};

/// Tile that dead ghosts head to before entering the house.
constexpr GridPosition DEAD_TARGET{13, 11};
/// @}

/// @name Tunnel
/// Ghosts are slow on the tunnel row for x <= TUNNEL_LEFT_END and
/// x >= TUNNEL_RIGHT_START.
/// @{
constexpr int TUNNEL_ROW = 14;
constexpr int TUNNEL_LEFT_END = 5;
constexpr int TUNNEL_RIGHT_START = 22;
/// @}

/// Row where the "READY!" and "GAME OVER" texts are drawn.
constexpr float BANNER_ROW = 17.0f;

/// @name Ghost AI
/// @{
/// Blinky's corner in scatter mode. It is outside the maze on purpose, so he
/// can never reach it and keeps circling the top-right block.
constexpr GridPosition BLINKY_SCATTER_TARGET{25, -3};

/// Tiles where ghosts that are chasing or scattering cannot turn up: the two
/// T-junctions above the ghost house and the two above Pac-Man's start.
constexpr std::array<GridPosition, 4> NO_UP_TILES{
    {{12, 11}, {15, 11}, {12, 23}, {15, 23}}};
/// @}

} // namespace MazeLayout

#endif
