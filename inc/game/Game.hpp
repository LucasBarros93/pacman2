#ifndef GAME_H
#define GAME_H

#include "entities/Pacman.hpp"
#include "game/FruitSpawner.hpp"
#include "game/GameState.hpp"
#include "graphics/Renderer.hpp"
#include "maze/Maze.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Game {
  private:
    sf::RenderWindow window;
    GameState gameState;
    FruitSpawner fruitSpawner;
    Maze maze;
    Pacman pacman;
    Renderer renderer;

  public:
    Game();
    Direction getInput();
    void run();
};

#endif
