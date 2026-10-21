#ifndef GAME_H
#define GAME_H

#include "entities/Pacman.hpp"
#include "game/GameState.hpp"
#include "graphics/Renderer.hpp"
#include "maze/Maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Game {
  private:
    sf::RenderWindow window;
    GameState gameState;
    Pacman pacman;
    Renderer renderer;
    Maze maze;

  public:
    Game();
    Direction getInput();
    void run();
};

#endif
