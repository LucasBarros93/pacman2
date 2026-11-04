#ifndef GAME_H
#define GAME_H

#include "entities/Pacman.hpp"
#include "game/FruitSpawner.hpp"
#include "game/GameState.hpp"
#include "game/Leaderboard.hpp"
#include "graphics/MenuRenderer.hpp"
#include "graphics/Renderer.hpp"
#include "maze/Maze.hpp"
#include "menus/Menu.hpp"
#include "menus/MenuController.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

class Game {
  private:
    sf::RenderWindow window;
    GameState gameState;
    FruitSpawner fruitSpawner;
    Maze maze;
    Pacman pacman;
    Leaderboard leaderboard;
    Menu menu;
    MenuController menuController;
    Renderer renderer;
    MenuRenderer menuRenderer;

  public:
    Game();
    Direction getInput();
    void run();
};

#endif
