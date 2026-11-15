#ifndef SPRITE_DATA_HPP
#define SPRITE_DATA_HPP

#include "core/Core.hpp"
#include "entities/Fruit.hpp"

#include <SFML/Graphics/Rect.hpp>

/// @brief Where each sprite is in assets/images/spritesheet.png.
///
/// The sheet is a grid of SPRITE_SIZE x SPRITE_SIZE cells. Every function
/// returns the rectangle of the sheet to give to sf::Sprite::setTextureRect().
namespace SpriteData {

constexpr int SPRITE_SIZE = 32; ///< Side of a cell of the sheet, in pixels.

/// @brief Pac-Man walking.
/// @param direction Where he looks. Direction::Neutral gives the full circle.
/// @param frame Animation frame, 0 or 1. Frame 0 is also the one used while
///        he stands still.
sf::IntRect pacman(Direction direction, int frame);

/// @brief The small Pac-Man that represents a spare life in the HUD.
sf::IntRect pacmanLife();

/// @brief One frame of Pac-Man's death animation.
/// @param frame From 0 to 11.
sf::IntRect pacmanDeath(int frame);

/// @brief A ghost walking.
/// @param type Which ghost.
/// @param direction Where it looks. Direction::Neutral looks left.
/// @param frame Animation frame, 0 or 1.
sf::IntRect ghost(GhostType type, Direction direction, int frame);

/// @brief A frightened ghost.
/// @param white true for the white version (flashing), false for the blue one.
/// @param frame Animation frame, 0 or 1.
sf::IntRect ghostFrightened(bool white, int frame);

/// @brief The eyes of a ghost that was eaten.
/// @param direction Where they look. Direction::Neutral looks left.
sf::IntRect ghostDead(Direction direction);

/// @brief A bonus fruit.
/// @param type Which fruit.
sf::IntRect fruit(FruitType type);

/// @brief The number shown where a ghost or a fruit was eaten.
/// @param points 200, 400, 800 or 1600 (ghosts), or the value of a fruit
///        (see scoreFor()).
/// @return The sprite, or an empty rectangle for any other value.
sf::IntRect scorePopup(int points);

} // namespace SpriteData

#endif
