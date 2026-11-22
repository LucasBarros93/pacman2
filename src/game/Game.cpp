#include "game/Game.hpp"
#include "core/Core.hpp"
#include "core/Difficulty.hpp"
#include "entities/Blinky.hpp"
#include "entities/Clyde.hpp"
#include "entities/Fruit.hpp"
#include "entities/Inky.hpp"
#include "entities/Pinky.hpp"
#include "game/GhostHouse.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <algorithm>

namespace {
constexpr unsigned int WINDOW_WIDTH = 800;
constexpr unsigned int WINDOW_HEIGHT = 600;
constexpr unsigned int FRAME_RATE_LIMIT = 60;

/// Longest time simulated in one frame, so a stall (like dragging the window)
/// does not make characters jump through walls.
constexpr float MAX_DELTA_TIME = 0.05f;

constexpr const char *MAP_PATH = "./assets/maps/map.txt";
constexpr const char *LEADERBOARD_PATH = "./assets/leaderboard/scores.txt";

// timings, in seconds
constexpr float READY_DURATION = 1.0f; ///< "READY!" at the start of a game.
constexpr float CLEAR_PAUSE = 0.8f;    ///< Stillness before the maze flashes.
constexpr float FLASH_INTERVAL =
    0.25f;                     ///< Each white or normal phase of the flashing.
constexpr int FLASH_COUNT = 3; ///< How many times the maze flashes.
constexpr float DEATH_FREEZE = 0.8f; ///< Stillness before the death animation.
constexpr float DEATH_FRAME_TIME =
    0.12f;                              ///< Each frame of the death animation.
constexpr int DEATH_FRAMES = 12;        ///< Frames of the death animation.
constexpr float DEATH_END_PAUSE = 0.6f; ///< Empty maze after the animation.
constexpr float GAME_OVER_INPUT_DELAY =
    0.5f; ///< Keys are ignored for a moment on "GAME OVER".
constexpr float GHOST_FREEZE_TIME = 0.8f; ///< Freeze after eating a ghost.
constexpr float FRUIT_POPUP_TIME =
    2.0f; ///< How long a fruit's points stay on screen.

// Pac-Man stops for 1 frame per pellet and 3 per power pellet (at 60 FPS).
// The value is half a frame below the number of frames on purpose, so the
// pause does not depend on small variations of the frame time.
constexpr float PELLET_STALL = 0.5f / 60.0f;
constexpr float POWER_PELLET_STALL = 2.5f / 60.0f;

// points
constexpr int PELLET_POINTS = 10;
constexpr int POWER_PELLET_POINTS = 50;
constexpr int FIRST_GHOST_POINTS =
    200; ///< Each next ghost of the same power pellet doubles it.
constexpr int MAX_GHOST_DOUBLINGS = 3; ///< 200, 400, 800, 1600.

/// The direction asked by the arrow keys or WASD, or Direction::Neutral if
/// none is held. With several keys held, up wins over down, then left, then
/// right.
Direction readDirectionKeys() {
    using Key = sf::Keyboard::Key;

    if (sf::Keyboard::isKeyPressed(Key::Up) ||
        sf::Keyboard::isKeyPressed(Key::W))
        return Direction::Up;

    if (sf::Keyboard::isKeyPressed(Key::Down) ||
        sf::Keyboard::isKeyPressed(Key::S))
        return Direction::Down;

    if (sf::Keyboard::isKeyPressed(Key::Left) ||
        sf::Keyboard::isKeyPressed(Key::A))
        return Direction::Left;

    if (sf::Keyboard::isKeyPressed(Key::Right) ||
        sf::Keyboard::isKeyPressed(Key::D))
        return Direction::Right;

    return Direction::Neutral;
}
} // namespace

Game::Game()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Pacman"),
      maze(MAP_PATH), leaderboard(LEADERBOARD_PATH),
      menuController(menu, leaderboard), renderer(window),
      menuRenderer(window) {
    window.setFramerateLimit(FRAME_RATE_LIMIT);
    window.setKeyRepeatEnabled(false); // one KeyPressed per press

    // The order of this vector is the order the ghosts leave the house
    // (Blinky starts outside).
    ghosts.push_back(std::make_unique<Blinky>(pacman));
    const Ghost &blinky = *ghosts.back();
    ghosts.push_back(std::make_unique<Pinky>(pacman));
    ghosts.push_back(std::make_unique<Inky>(pacman, blinky));
    ghosts.push_back(std::make_unique<Clyde>(pacman));
}

void Game::handleEvents() {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }

        // The arrow keys are read by polling, which also works while another
        // window has the focus, so the game pauses when it loses it.
        if (event->is<sf::Event::FocusLost>() && mode != GameMode::Paused)
            togglePause();

        if (mode == GameMode::Menu) {
            switch (menuController.handleEvent(*event)) {
            case Menu::Action::Play:
                startNewGame();
                break;
            case Menu::Action::Quit:
                window.close();
                break;
            case Menu::Action::None:
                break;
            }
        } else if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
            const auto code = key->code;

            if (mode == GameMode::GameOver) {
                const bool confirm = code == sf::Keyboard::Key::Escape ||
                                     code == sf::Keyboard::Key::Enter ||
                                     code == sf::Keyboard::Key::Space;

                if (confirm && modeTimer >= GAME_OVER_INPUT_DELAY)
                    finishGame();
            } else if (code == sf::Keyboard::Key::P ||
                       code == sf::Keyboard::Key::Escape) {
                togglePause();
            }
        }
    }
}

void Game::startNewGame() {
    gameState.reset();
    gameState.setHighScore(leaderboard.getTopScore());
    resetRound();
    startReady();
}

void Game::startNextRound() {
    gameState.advanceLevel();
    resetRound();
    mode = GameMode::Playing;
}

void Game::startReady() {
    mode = GameMode::Ready;
    modeTimer = 0.0f;
    renderer.setShowReady(true);
}

void Game::startClearing() {
    mode = GameMode::Clearing;
    modeTimer = 0.0f;
    pacman.stop();
    fruitSpawner.resetRound();
    popups.clear();
}

void Game::startGhostFreeze(Ghost &ghost, int points) {
    ghost.eat();
    ghost.setVisible(false);
    pacman.setVisible(false);

    popups.push_back({ghost.getPosition(), points, GHOST_FREEZE_TIME});

    mode = GameMode::EatingGhost;
    modeTimer = 0.0f;
}

void Game::startGameOver() {
    mode = GameMode::GameOver;
    modeTimer = 0.0f;
    renderer.setShowGameOver(true);
}

void Game::killPacman() {
    for (const auto &ghost : ghosts)
        ghost->setVisible(false);

    mode = GameMode::Dying;
    modeTimer = 0.0f;
    fruitSpawner.resetRound();
    popups.clear();
}

void Game::finishDeath() {
    renderer.setPacmanDeathFrame(-1);
    gameState.loseLife();

    if (gameState.isGameOver()) {
        startGameOver();
        return;
    }

    resetActors();

    // after the reset: Blinky's Elroy stays off until Clyde leaves
    for (const auto &ghost : ghosts)
        ghost->onPacmanDied();

    startReady();
}

void Game::finishGame() {
    renderer.setMazeFlash(false);
    renderer.setShowReady(false);
    renderer.setShowGameOver(false);
    renderer.setPaused(false);
    renderer.setPacmanDeathFrame(-1);

    menuController.onGameOver(gameState.getScore());
    mode = GameMode::Menu;
}

void Game::togglePause() {
    if (mode == GameMode::Paused) {
        mode = modeBeforePause;
        renderer.setPaused(false);
    } else if (mode == GameMode::Ready || mode == GameMode::Playing ||
               mode == GameMode::Clearing || mode == GameMode::Dying ||
               mode == GameMode::EatingGhost) {
        modeBeforePause = mode;
        mode = GameMode::Paused;
        renderer.setPaused(true);
    }
}

void Game::resetRound() {
    maze.reset();
    resetActors();
    fruitSpawner.resetRound();
    dotsEatenThisRound = 0;
    renderer.setMazeFlash(false);
}

void Game::resetActors() {
    const int round = gameState.getRound();
    pacman.reset();
    pacman.setRound(round);

    ghostHouse.reset(round);

    for (const auto &ghost : ghosts) {
        ghost->reset();
        ghost->setRound(round);
    }

    popups.clear();
}

void Game::frightenGhosts() {
    ghostsEatenInRow = 0;

    const float duration = Difficulty::frightenedDuration(gameState.getRound());
    for (const auto &ghost : ghosts)
        ghost->frighten(duration);
}

void Game::collectPellet() {
    const auto collectible = maze.consumeCollectible(pacman.getPosition());
    if (!collectible)
        return;

    const bool isPowerPellet = *collectible == CellType::PowerPellet;

    gameState.addScore(isPowerPellet ? POWER_PELLET_POINTS : PELLET_POINTS);
    ++dotsEatenThisRound;
    fruitSpawner.onDotEaten(dotsEatenThisRound, gameState.getRound());
    pacman.stall(isPowerPellet ? POWER_PELLET_STALL : PELLET_STALL);

    if (isPowerPellet)
        frightenGhosts();
}

void Game::collectFruit() {
    const auto fruit = fruitSpawner.tryCollect(pacman.getPosition());
    if (!fruit)
        return;

    const int points = scoreFor(fruit->type);
    gameState.addScore(points);
    popups.push_back({fruit->position, points, FRUIT_POPUP_TIME});
}

bool Game::handleGhostCollisions() {
    for (const auto &ghost : ghosts) {
        if (!Collision::overlaps(pacman.getPosition(), ghost->getPosition(),
                                 Collision::GHOST_RADIUS))
            continue;

        if (ghost->isEdible()) {
            const int points = FIRST_GHOST_POINTS << std::min(
                                   ghostsEatenInRow, MAX_GHOST_DOUBLINGS);
            gameState.addScore(points);
            ++ghostsEatenInRow;
            startGhostFreeze(*ghost, points);
            return true;
        }

        if (ghost->isDeadly()) {
            killPacman();
            return true;
        }
    }

    return false;
}

void Game::updateReady(float deltaTime) {
    modeTimer += deltaTime;

    if (modeTimer >= READY_DURATION) {
        renderer.setShowReady(false);
        mode = GameMode::Playing;
    }
}

void Game::updatePlaying(float deltaTime) {
    updatePopups(deltaTime);

    pacman.trySetDirection(readDirectionKeys(), maze);
    pacman.update(maze, deltaTime);

    ghostHouse.update(deltaTime, ghosts);
    for (const auto &ghost : ghosts)
        ghost->update(maze, deltaTime);

    if (handleGhostCollisions())
        return; // Pac-Man died or ate a ghost: the mode changed

    fruitSpawner.update(deltaTime);
    collectPellet();
    collectFruit();

    if (maze.getRemainingCollectibles() == 0)
        startClearing();
}

void Game::updateClearing(float deltaTime) {
    modeTimer += deltaTime;

    const float flashTime = modeTimer - CLEAR_PAUSE;
    const bool white = flashTime >= 0.0f &&
                       static_cast<int>(flashTime / FLASH_INTERVAL) % 2 == 0;

    renderer.setMazeFlash(white);

    if (flashTime >= 2 * FLASH_COUNT * FLASH_INTERVAL)
        startNextRound();
}

void Game::updateDying(float deltaTime) {
    modeTimer += deltaTime;

    const float animationTime = modeTimer - DEATH_FREEZE;
    if (animationTime < 0.0f)
        return; // still frozen

    const int frame = static_cast<int>(animationTime / DEATH_FRAME_TIME);

    if (frame < DEATH_FRAMES)
        renderer.setPacmanDeathFrame(frame);
    else
        pacman.setVisible(false);

    if (animationTime >= DEATH_FRAMES * DEATH_FRAME_TIME + DEATH_END_PAUSE)
        finishDeath();
}

void Game::updateGameOver(float deltaTime) { modeTimer += deltaTime; }

void Game::updateEatingGhost(float deltaTime) {
    modeTimer += deltaTime;
    updatePopups(deltaTime);

    if (modeTimer < GHOST_FREEZE_TIME)
        return;

    for (const auto &ghost : ghosts)
        ghost->setVisible(true);

    pacman.setVisible(true);
    mode = GameMode::Playing;
}

void Game::updatePopups(float deltaTime) {
    for (ScorePopup &popup : popups)
        popup.remaining -= deltaTime;

    popups.erase(std::remove_if(popups.begin(), popups.end(),
                                [](const ScorePopup &popup) {
                                    return popup.remaining <= 0.0f;
                                }),
                 popups.end());
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        const float deltaTime =
            std::min(clock.restart().asSeconds(), MAX_DELTA_TIME);

        handleEvents();

        if (!window.isOpen())
            break;

        if (mode == GameMode::Menu) {
            menuRenderer.render(menu, leaderboard);
            continue;
        }

        switch (mode) {
        case GameMode::Ready:
            updateReady(deltaTime);
            break;
        case GameMode::Playing:
            updatePlaying(deltaTime);
            break;
        case GameMode::Clearing:
            updateClearing(deltaTime);
            break;
        case GameMode::Dying:
            updateDying(deltaTime);
            break;
        case GameMode::GameOver:
            updateGameOver(deltaTime);
            break;
        case GameMode::EatingGhost:
            updateEatingGhost(deltaTime);
            break;
        case GameMode::Menu:
        case GameMode::Paused:
            break; // Paused: nothing is updated, the frozen frame is redrawn
        }

        renderer.render(pacman, ghosts, popups, gameState, fruitSpawner, maze);
    }
}
