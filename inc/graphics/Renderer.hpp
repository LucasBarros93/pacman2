#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "entities/Pacman.hpp"
#include "game/GameState.hpp"
#include "maze/Maze.hpp"

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
    void renderHudTop(const GameState &gameState, const Maze &maze);
    void renderHudBottom(const GameState &gameState, const Maze &maze);

  public:
    Renderer(sf::RenderWindow &window);

    void render(const Pacman &pacman, const GameState &gameState,
                const Maze &maze);
};

#endif
