#ifndef SPRITE_DATA_HPP
#define SPRITE_DATA_HPP

#include "core/Core.hpp"
#include "entities/Fruit.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace SpriteData {
static constexpr float CELL_SIZE = 24.0f;
static constexpr int SPRITE_SIZE = 16;

sf::IntRect pacman(int frame, Direction direction);
sf::IntRect pacmanLife();
sf::IntRect pacmanDeath(int frame);
sf::IntRect ghost(GhostType type, Direction direction, int frame);
sf::IntRect ghostFrightened(bool white, int frame);
sf::IntRect ghostDead(Direction direction);
sf::IntRect fruit(FruitType fruit);

} // namespace SpriteData

namespace Layout {

constexpr float HUD_TOP_HEIGHT = 40.f;
constexpr float HUD_BOTTOM_HEIGHT = 30.f;

} // namespace Layout

#endif
