#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "entities/Pacman.hpp"
#include <SFML/Graphics.hpp>

class Renderer {
  private:
    sf::RenderWindow &window;

  public:
    Renderer(sf::RenderWindow &window);

    void render(const Pacman &pacman);
};

#endif
