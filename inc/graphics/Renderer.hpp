#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "entities/Pacman.hpp"
#include <SFML/Graphics.hpp>

class Renderer {
  private:
    sf::RenderWindow &window;

    sf::Texture spritesheet;
    sf::Texture mazeTexture;
    sf::Font font;

    sf::View hudTopView;
    sf::View gameView;
    sf::View hudBottomView;

    sf::Clock animationClock;
    int animationFrame;

    sf::Vector2f getMazeOffset(const Maze &maze) const;
    float getMazeScale(const Maze &maze) const;

    void renderPacman(const Pacman &pacman, const Maze &maze);
    void renderMaze(const Maze &maze);
    void renderCollectibles(const Maze &maze);
    void renderHudTop(int score, int highScore, const Maze &maze);
    void renderHudBottom(int lives, const Maze &maze);

  public:
    Renderer(sf::RenderWindow &window);

    void render(const Maze &maze, const Pacman &pacman, int score,
                int highScore, int lives);
};

#endif
