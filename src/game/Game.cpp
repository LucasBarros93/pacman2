#include "game/Game.hpp"
#include "core/Core.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

namespace {
constexpr float CLEAR_PAUSE = 0.8f;
constexpr float FLASH_INTERVAL = 0.25f;
constexpr int FLASH_COUNT = 3;
constexpr float READY_DURATION = 1.0f;
constexpr float DEATH_FREEZE = 0.8f;
constexpr float DEATH_FRAME_TIME = 0.12f;
constexpr int DEATH_FRAMES = 12;
constexpr float DEATH_END_PAUSE = 0.6f;
constexpr float GAME_OVER_INPUT_DELAY = 0.5f;
} // namespace

Game::Game()
    : window(sf::VideoMode({800, 600}), "Pacman"), gameState(),
      fruitSpawner({13.5f, 17.0f}), maze("./assets/maps/map.txt"),
      pacman({13.5f, 23.0f}), leaderboard("./assets/leaderboard/scores.txt"),
      menu(), menuController(menu, leaderboard), renderer(window),
      menuRenderer(window) {
    window.setFramerateLimit(60);
}

Direction Game::getInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        return Direction::Up;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        return Direction::Down;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        return Direction::Left;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        return Direction::Right;

    return Direction::Neutral;
}

void Game::handleEvents() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            return;
        }

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
            } else if (code == sf::Keyboard::Key::Escape) {
                if (mode == GameMode::Playing)
                    killPacman();
            } else if (code == sf::Keyboard::Key::P) {
                togglePause();
            }
        }
    }
}

void Game::resetRound() {
    maze.reset();
    pacman.reset();
    fruitSpawner.resetRound();
    dotsEatenThisRound = 0;
    renderer.setMazeFlash(false);
    mode = GameMode::Playing;
}

void Game::startReady() {
    mode = GameMode::Ready;
    modeTimer = 0.0f;
    renderer.setShowReady(true);
}

void Game::updateReady(float deltaTime) {
    modeTimer += deltaTime;

    if (modeTimer >= READY_DURATION) {
        renderer.setShowReady(false);
        mode = GameMode::Playing;
    }
}

void Game::togglePause() {
    if (mode == GameMode::Paused) {
        mode = modeBeforePause;
        renderer.setPaused(false);
    } else if (mode == GameMode::Ready || mode == GameMode::Playing ||
               mode == GameMode::Clearing || mode == GameMode::Dying) {
        modeBeforePause = mode;
        mode = GameMode::Paused;
        renderer.setPaused(true);
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
}

void Game::startClearing() {
    mode = GameMode::Clearing;
    modeTimer = 0.0f;
    pacman.stop();
    fruitSpawner.resetRound();
}

void Game::startGameOver() {
    mode = GameMode::GameOver;
    modeTimer = 0.0f;
    renderer.setShowGameOver(true);
}

void Game::finishGame() {
    renderer.setMazeFlash(false);
    renderer.setShowReady(false);
    renderer.setShowGameOver(false);
    renderer.setPaused(false);
    renderer.setPacmanDeathFrame(-1);
    renderer.setPacmanVisible(true);
    menuController.onGameOver(gameState.getScore());
    mode = GameMode::Menu;
}

void Game::killPacman() {
    mode = GameMode::Dying;
    modeTimer = 0.0f;
    fruitSpawner.resetRound();
}

void Game::finishDeath() {
    renderer.setPacmanDeathFrame(-1);
    gameState.loseLife();

    if (gameState.getLives() <= 0) {
        startGameOver();
        return;
    }

    renderer.setPacmanVisible(true);
    pacman.reset();
    startReady();
}

void Game::updatePlaying(float deltaTime) {
    pacman.trySetDirection(getInput(), maze);
    pacman.update(maze, deltaTime);
    fruitSpawner.update(deltaTime);

    if (const auto collectible = maze.consumeCollectible(
            pacman.getPosition(), Collision::PELLET_RADIUS)) {
        gameState.addScore(*collectible == CellType::PowerPellet ? 50 : 10);
        ++dotsEatenThisRound;
        fruitSpawner.onDotEaten(dotsEatenThisRound, gameState.getRound());
    }

    if (const auto points = fruitSpawner.tryCollect(pacman.getPosition()))
        gameState.addScore(*points);

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

    const float animTime = modeTimer - DEATH_FREEZE;
    if (animTime < 0.0f)
        return;

    const int frame = static_cast<int>(animTime / DEATH_FRAME_TIME);

    if (frame < DEATH_FRAMES)
        renderer.setPacmanDeathFrame(frame);
    else
        renderer.setPacmanVisible(false);

    if (animTime >= DEATH_FRAMES * DEATH_FRAME_TIME + DEATH_END_PAUSE)
        finishDeath();
}

void Game::updateGameOver(float deltaTime) { modeTimer += deltaTime; }

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        const float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);

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
        case GameMode::Menu:
        case GameMode::Paused:
            break;
        }

        renderer.render(pacman, gameState, fruitSpawner, maze);
    }
}
