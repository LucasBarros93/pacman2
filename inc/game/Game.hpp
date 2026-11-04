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

enum class GameMode { Menu, Ready, Playing, Clearing, Paused };

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

    GameMode mode = GameMode::Menu;
    GameMode modeBeforePause = GameMode::Playing;

    int dotsEatenThisRound = 0;
    float modeTimer = 0.0f;

    void handleEvents();

    void startNewGame();
    void startNextRound();
    void startClearing();
    void finishGame();
    void resetRound();
    void startReady();
    void togglePause();

    void updateReady(float deltaTime);
    void updatePlaying(float deltaTime);
    void updateClearing(float deltaTime);

  public:
    Game();
    Direction getInput();
    void run();
};

#endif
