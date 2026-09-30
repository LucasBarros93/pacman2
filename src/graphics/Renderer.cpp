#include "graphics/Renderer.hpp"
#include "graphics/SpriteData.hpp"

Renderer::Renderer(sf::RenderWindow &window) : window{window} {
    if (!spritesheet.loadFromFile("assets/images/spritesheet.png")) {
        throw std::runtime_error("Could not load spritesheet");
    }
};

void Renderer::renderPacman(const Pacman &pacman) {
    if (this->animationClock.getElapsedTime().asMilliseconds() > 200) {
        this->animationFrame = (this->animationFrame + 1) % 2;
        this->animationClock.restart();
    }
    sf::Sprite sprite{spritesheet};

    switch (pacman.getDirection()) {
    case Direction::Right:
        sprite.setTextureRect(SpriteData::pacmanRight(animationFrame));
        break;

    case Direction::Left:
        sprite.setTextureRect(SpriteData::pacmanLeft(animationFrame));
        break;

    case Direction::Up:
        sprite.setTextureRect(SpriteData::pacmanUp(animationFrame));
        break;

    case Direction::Down:
        sprite.setTextureRect(SpriteData::pacmanDown(animationFrame));
        break;

    case Direction::Neutral:
        sprite.setTextureRect(SpriteData::pacmanNeutral());
        break;
    }

    Position position = pacman.getPosition();
    sprite.setPosition({position.first, position.second});

    sprite.setScale({3.0f, 3.0f});
    window.draw(sprite);
}
