#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "entities/Pacman.hpp"
#include <SFML/Graphics.hpp>

class Renderer {
  private:
    sf::RenderWindow &window;
    sf::Texture spritesheet;
    sf::Clock animationClock;
    int animationFrame;

  public:
    Renderer(sf::RenderWindow &window);

    void renderPacman(const Pacman &pacman);
    void renderMaze(const Maze &maze);
};

#endif
