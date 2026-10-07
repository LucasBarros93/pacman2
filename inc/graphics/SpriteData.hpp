#ifndef SPRITE_DATA_HPP
#define SPRITE_DATA_HPP

#include <SFML/Graphics/Rect.hpp>

namespace SpriteData {
static constexpr float CELL_SIZE = 24.0f;
static constexpr int SPRITE_SIZE = 16;

sf::IntRect pacmanRight(int frame);
sf::IntRect pacmanLeft(int frame);
sf::IntRect pacmanUp(int frame);
sf::IntRect pacmanDown(int frame);
sf::IntRect pacmanNeutral();

} // namespace SpriteData

#endif
