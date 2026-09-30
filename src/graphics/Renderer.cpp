#include "graphics/Renderer.hpp"

Renderer::Renderer(sf::RenderWindow &window) : window{window} {
    if (!spritesheet.loadFromFile("assets/images/spritesheet.png")) {
        throw std::runtime_error("Could not load spritesheet");
    }
};

void Renderer::renderPacman(const Pacman &pacman) {
    sf::Sprite sprite{spritesheet};

    sprite.setTextureRect(sf::IntRect({0, 0}, {16, 16}));

    Position position = pacman.getPosition();
    sprite.setPosition({position.first, position.second});

    sprite.setScale({3.0f, 3.0f});
    window.draw(sprite);
}
