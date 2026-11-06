#include "graphics/SpriteData.hpp"
#include "core/Core.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace SpriteData {

sf::IntRect pacman(int frame, Direction direction) {
    switch (direction) {
    case Direction::Right:
        return {{frame * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};

    case Direction::Left:
        return {{frame * SPRITE_SIZE, SPRITE_SIZE}, {SPRITE_SIZE, SPRITE_SIZE}};

    case Direction::Up:
        return {{frame * SPRITE_SIZE, 2 * SPRITE_SIZE},
                {SPRITE_SIZE, SPRITE_SIZE}};

    case Direction::Down:
        return {{frame * SPRITE_SIZE, 3 * SPRITE_SIZE},
                {SPRITE_SIZE, SPRITE_SIZE}};

    case Direction::Neutral:
        return {{2 * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};
    }

    return {{2 * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};
}
sf::IntRect pacmanLife() {
    return {{8 * SPRITE_SIZE, SPRITE_SIZE}, {SPRITE_SIZE, SPRITE_SIZE}};
}
sf::IntRect pacmanDeath(int frame) {
    return {{(frame + 2) * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};
}

sf::IntRect fruit(int fruit) {
    return {{(1 + fruit) * SPRITE_SIZE, 3 * SPRITE_SIZE},
            {SPRITE_SIZE, SPRITE_SIZE}};
}

} // namespace SpriteData
