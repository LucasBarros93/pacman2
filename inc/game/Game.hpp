#ifndef GAME_HPP
#define GAME_HPP

#include "entities/Ghost.hpp"
#include "entities/Pacman.hpp"
#include "game/FruitSpawner.hpp"
#include "game/GameState.hpp"
#include "game/Leaderboard.hpp"
#include "game/ScorePopup.hpp"
#include "graphics/MenuRenderer.hpp"
#include "graphics/Renderer.hpp"
#include "maze/Maze.hpp"
#include "menus/Menu.hpp"
#include "menus/MenuController.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>
#include <vector>

/// @brief The game: owns the window and everything in it, and runs the main
///        loop.
///
/// What each frame does depends on the current mode:
/// - Menu: the menus. Choosing "play" starts a game.
/// - Ready: "READY!" at the start of a game, everything still.
/// - Playing: the game itself. It can go to Paused, Clearing (all pellets
///   eaten), EatingGhost or Dying (Pac-Man was caught).
/// - Clearing: the maze flashes, then the next round starts.
/// - EatingGhost: a short freeze that shows the points of the ghost just eaten.
/// - Dying: Pac-Man's death animation. Then the game goes back to Ready, or
///   to GameOver if there are no lives left.
/// - GameOver: waits for a key, then shows the name entry (if the score is a
///   record) or the main menu.
class Game {
  private:
    /// @brief What the game is doing.
    enum class GameMode {
        Menu,       ///< Main menu, leaderboard or name entry.
        Ready,      ///< "READY!" at the start of a game.
        Playing,    ///< The game itself.
        Clearing,   ///< All pellets eaten: the maze flashes.
        Paused,     ///< Frozen by the player.
        Dying,      ///< Pac-Man was caught: freeze, then the death animation.
        GameOver,   ///< "GAME OVER", until the player presses a key.
        EatingGhost ///< Short freeze after eating a ghost.
    };

    sf::RenderWindow window;
    GameState gameState;
    FruitSpawner fruitSpawner;
    Maze maze;
    Pacman pacman;
    std::vector<std::unique_ptr<Ghost>>
        ghosts; ///< In the order they leave the house.
    Leaderboard leaderboard;
    Menu menu;
    MenuController menuController;
    Renderer renderer;
    MenuRenderer menuRenderer;
    std::vector<ScorePopup> popups; ///< Scores being shown on the maze.

    GameMode mode = GameMode::Menu;
    GameMode modeBeforePause =
        GameMode::Playing; ///< Where to go back to when unpausing.

    int dotsEatenThisRound =
        0; ///< Kept when Pac-Man dies, so fruits do not repeat.
    float modeTimer =
        0.0f; ///< Seconds in the current mode (for the timed ones).
    float ghostReleaseTimer =
        0.0f;                 ///< Seconds since the last ghost left the house.
    int ghostsEatenInRow = 0; ///< Ghosts eaten with the current power pellet.

    /// @brief Handles every pending window event (menus, pause, closing).
    void handleEvents();

    /// @name Changes of mode
    /// @{
    void startNewGame();
    void startNextRound();
    void startReady();
    void startClearing();
    void startGhostFreeze(Ghost &ghost, int points);
    void startGameOver();
    void killPacman();
    void finishDeath();
    void finishGame();
    void togglePause();
    /// @}

    /// @brief Puts the maze, Pac-Man, ghosts and fruit back for a round.
    void resetRound();

    /// @brief Puts Pac-Man and the ghosts back at their starting places.
    void resetActors();

    /// @brief Frightens every ghost (a power pellet was eaten).
    void frightenGhosts();

    /// @brief Lets Pac-Man eat the pellet he is on, if any.
    void collectPellet();

    /// @brief Lets Pac-Man eat the fruit he is on, if any.
    void collectFruit();

    /// @brief Handles Pac-Man touching a ghost: he eats it if it is frightened
    ///        and dies if it is not.
    /// @return true if something happened (the mode changed).
    bool handleGhostCollisions();

    /// @name Updates, one per mode
    /// @{
    void updateReady(float deltaTime);
    void updatePlaying(float deltaTime);
    void updateClearing(float deltaTime);
    void updateDying(float deltaTime);
    void updateGameOver(float deltaTime);
    void updateEatingGhost(float deltaTime);
    /// @}

    /// @brief Lets the next waiting ghost out when its time comes.
    void updateGhostRelease(float deltaTime);

    /// @brief Counts down the popups and removes the ones that ended.
    void updatePopups(float deltaTime);

  public:
    /// @throws std::runtime_error If the map, the images or the font cannot
    ///         be loaded.
    Game();

    /// @brief Runs the game until the window is closed.
    void run();
};

#endif
