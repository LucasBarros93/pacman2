#ifndef SPRITE_DATA_HPP
#define SPRITE_DATA_HPP

#include <SFML/Graphics/Rect.hpp>

namespace SpriteData {

constexpr int SPRITE_SIZE = 16;

sf::IntRect pacmanRight(int frame);
sf::IntRect pacmanLeft(int frame);
sf::IntRect pacmanUp(int frame);
sf::IntRect pacmanDown(int frame);
sf::IntRect pacmanNeutral();

} // namespace SpriteData

#endif
