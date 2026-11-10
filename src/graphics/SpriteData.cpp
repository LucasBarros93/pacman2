#include "graphics/SpriteData.hpp"
#include "core/Core.hpp"
#include "entities/Fruit.hpp"

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
        break;
    }

    return {{2 * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};
}
sf::IntRect pacmanLife() {
    return {{8 * SPRITE_SIZE, SPRITE_SIZE}, {SPRITE_SIZE, SPRITE_SIZE}};
}
sf::IntRect pacmanDeath(int frame) {
    return {{(frame + 2) * SPRITE_SIZE, 0}, {SPRITE_SIZE, SPRITE_SIZE}};
}

sf::IntRect ghost(GhostType type, Direction direction, int frame) {
    int directionIndex = 1;
    switch (direction) {
    case Direction::Right:
        directionIndex = 0;
        break;
    case Direction::Left:
        directionIndex = 1;
        break;
    case Direction::Up:
        directionIndex = 2;
        break;
    case Direction::Down:
        directionIndex = 3;
        break;
    case Direction::Neutral:
        break;
    }

    const int column = directionIndex * 2 + frame;
    const int row = static_cast<int>(type) + 4;

    return sf::IntRect({column * SPRITE_SIZE, row * SPRITE_SIZE},
                       {SPRITE_SIZE, SPRITE_SIZE});
}

sf::IntRect ghostFrightened(bool white, int frame) {
    const int column = 8 + (white ? 2 : 0) + frame;

    return sf::IntRect({column * SPRITE_SIZE, 4 * SPRITE_SIZE},
                       {SPRITE_SIZE, SPRITE_SIZE});
}

sf::IntRect ghostDead(Direction direction) {
    int directionIndex = 1;
    switch (direction) {
    case Direction::Right:
        directionIndex = 0;
        break;
    case Direction::Left:
        directionIndex = 1;
        break;
    case Direction::Up:
        directionIndex = 2;
        break;
    case Direction::Down:
        directionIndex = 3;
        break;
    case Direction::Neutral:
        break;
    }

    const int column = directionIndex + 8;

    return sf::IntRect({column * SPRITE_SIZE, 5 * SPRITE_SIZE},
                       {SPRITE_SIZE, SPRITE_SIZE});
}

sf::IntRect fruit(FruitType fruit) {
    const int column = static_cast<int>(fruit);
    return {{(1 + column) * SPRITE_SIZE, 3 * SPRITE_SIZE},
            {SPRITE_SIZE, SPRITE_SIZE}};
}

sf::IntRect scorePopup(int points) {
    int row, column;
    switch (points) {
    case 200:
        row = 8;
        column = 0;
        break;
    case 400:
        row = 8;
        column = 1;
        break;
    case 800:
        row = 8;
        column = 2;
        break;
    case 1600:
        row = 8;
        column = 3;
        break;

    case 100:
        row = 9;
        column = 0;
        break;
    case 300:
        row = 9;
        column = 1;
        break;
    case 500:
        row = 9;
        column = 2;
        break;
    case 700:
        row = 9;
        column = 3;
        break;
    case 1000:
        row = 9;
        column = 4;
        return sf::IntRect({column * SPRITE_SIZE, row * SPRITE_SIZE},
                           {SPRITE_SIZE + 4, SPRITE_SIZE});
    case 2000:
        row = 10;
        column = 4;
        return sf::IntRect({column * SPRITE_SIZE - 4, row * SPRITE_SIZE},
                           {SPRITE_SIZE + 8, SPRITE_SIZE});
    case 3000:
        row = 11;
        column = 4;
        return sf::IntRect({column * SPRITE_SIZE - 4, row * SPRITE_SIZE},
                           {SPRITE_SIZE + 8, SPRITE_SIZE});
    case 5000:
        row = 12;
        column = 4;
        return sf::IntRect({column * SPRITE_SIZE - 4, row * SPRITE_SIZE},
                           {SPRITE_SIZE + 8, SPRITE_SIZE});

    default:
        return sf::IntRect();
    }
    return sf::IntRect({column * SPRITE_SIZE, row * SPRITE_SIZE},
                       {SPRITE_SIZE, SPRITE_SIZE});
}

} // namespace SpriteData
