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
            if (key->code == sf::Keyboard::Key::Escape)
                finishGame();
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

void Game::startNewGame() {
    gameState.reset();
    gameState.setHighScore(leaderboard.getTopScore());
    resetRound();
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

void Game::finishGame() {
    renderer.setMazeFlash(false);
    menuController.onGameOver(gameState.getScore());
    mode = GameMode::Menu;
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

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        const float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);

        handleEvents();

        if (!window.isOpen())
            break;

        switch (mode) {
        case GameMode::Menu:
            menuRenderer.render(menu, leaderboard);
            break;

        case GameMode::Playing:
            updatePlaying(deltaTime);
            renderer.render(pacman, gameState, fruitSpawner, maze);
            break;

        case GameMode::Clearing:
            updateClearing(deltaTime);
            renderer.render(pacman, gameState, fruitSpawner, maze);
            break;
        }
    }
}
