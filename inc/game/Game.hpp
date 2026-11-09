#ifndef GAME_H
#define GAME_H

#include "entities/Ghost.hpp"
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

enum class GameMode {
    Menu,
    Ready,
    Playing,
    Clearing,
    Paused,
    Dying,
    GameOver,
    EatingGhost
};

class Game {
  private:
    sf::RenderWindow window;
    GameState gameState;
    FruitSpawner fruitSpawner;
    Maze maze;
    Pacman pacman;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    Leaderboard leaderboard;
    Menu menu;
    MenuController menuController;
    Renderer renderer;
    MenuRenderer menuRenderer;
    std::vector<ScorePopup> popups;

    GameMode mode = GameMode::Menu;
    GameMode modeBeforePause = GameMode::Playing;

    int dotsEatenThisRound = 0;
    float modeTimer = 0.0f;
    float ghostReleaseTimer = 0.0f;
    int ghostsEatenInRow = 0;

    void handleEvents();

    void startNewGame();
    void startNextRound();
    void startGameOver();
    void startClearing();
    void startGhostFreeze(Ghost &ghost, int points);
    void finishGame();
    void resetRound();
    void startReady();
    void togglePause();
    void killPacman();
    void finishDeath();
    void resetActors();
    void frightenGhosts();

    void updateReady(float deltaTime);
    void updatePlaying(float deltaTime);
    void updateClearing(float deltaTime);
    void updateDying(float deltaTime);
    void updateGameOver(float deltaTime);
    void updateGhostRelease(float deltaTime);
    void updateEatingGhost(float deltaTime);
    void updatePopups(float deltaTime);

  public:
    Game();
    Direction getInput();
    void run();
};

#endif
