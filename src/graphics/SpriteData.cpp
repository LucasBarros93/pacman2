#include "graphics/SpriteData.hpp"

namespace SpriteData {

sf::IntRect pacmanRight(int frame) {
    return {{frame * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};
}

sf::IntRect pacmanLeft(int frame) {
    return {{frame * SPRITE_SIZE, SPRITE_SIZE}, {SPRITE_SIZE, SPRITE_SIZE}};
}

sf::IntRect pacmanUp(int frame) {
    return {{frame * SPRITE_SIZE, 2 * SPRITE_SIZE}, {SPRITE_SIZE, SPRITE_SIZE}};
}

sf::IntRect pacmanDown(int frame) {
    return {{frame * SPRITE_SIZE, 3 * SPRITE_SIZE}, {SPRITE_SIZE, SPRITE_SIZE}};
}

sf::IntRect pacmanNeutral() {
    return {{2 * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};
}
sf::IntRect pacmanLife() {
    return {{8 * SPRITE_SIZE, SPRITE_SIZE}, {SPRITE_SIZE, SPRITE_SIZE}};
}

} // namespace SpriteData
