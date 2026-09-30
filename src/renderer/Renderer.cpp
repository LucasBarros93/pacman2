#include "renderer/Renderer.hpp"

Renderer::Renderer(sf::RenderWindow &window) : window{window} {};

void Renderer::render(const Pacman &pacman) {
    sf::CircleShape shape(20.0f);

    Position position = pacman.getPosition();
    shape.setPosition({position.first, position.second});

    window.draw(shape);
}
