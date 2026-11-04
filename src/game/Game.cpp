#include "game/Game.hpp"
#include "core/Core.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

namespace {
enum class Mode { Menu, Playing };

enum class RoundPhase { Playing, Clearing };

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

void Game::run() {
    sf::Clock clock;
    int dotsEatenThisRound = 0;

    Mode mode = Mode::Menu;
    RoundPhase phase = RoundPhase::Playing;
    float phaseTimer = 0.0f;

    auto startNewGame = [&]() {
        gameState.reset();
        gameState.setHighScore(leaderboard.getTopScore());
        maze.reset();
        pacman.reset();
        fruitSpawner.resetRound();
        dotsEatenThisRound = 0;
        renderer.setMazeFlash(false);
        phase = RoundPhase::Playing;
        mode = Mode::Playing;
    };

    auto startNextRound = [&]() {
        gameState.advanceLevel();
        maze.reset();
        pacman.reset();
        fruitSpawner.resetRound();
        dotsEatenThisRound = 0;
        renderer.setMazeFlash(false);
        phase = RoundPhase::Playing;
    };
    auto finishGame = [&]() {
        renderer.setMazeFlash(false);
        menuController.onGameOver(gameState.getScore());
        mode = Mode::Menu;
    };

    while (window.isOpen()) {
        const float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (mode == Mode::Menu) {
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
            } else if (const auto *key =
                           event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape)
                    finishGame();
            }
        }

        if (!window.isOpen())
            break;

        if (mode == Mode::Menu) {
            menuRenderer.render(menu, leaderboard);
            continue;
        }

        if (phase == RoundPhase::Playing) {
            Direction input = this->getInput();
            pacman.trySetDirection(input, this->maze);
            pacman.update(this->maze, deltaTime);

            fruitSpawner.update(deltaTime);

            const auto collectible = maze.consumeCollectible(
                pacman.getPosition(), Collision::PELLET_RADIUS);

            if (collectible) {
                switch (*collectible) {
                case CellType::Pellet:
                    gameState.addScore(10);
                    break;
                case CellType::PowerPellet:
                    gameState.addScore(50);
                    break;
                default:
                    break;
                }
                ++dotsEatenThisRound;
                fruitSpawner.onDotEaten(dotsEatenThisRound,
                                        gameState.getRound());
            }

            const auto points = fruitSpawner.tryCollect(pacman.getPosition());
            if (points.has_value())
                gameState.addScore(*points);

            if (maze.getRemainingCollectibles() == 0) {
                phase = RoundPhase::Clearing;
                phaseTimer = 0.0f;
                pacman.stop();
                fruitSpawner.resetRound();
            }
        } else {
            phaseTimer += deltaTime;

            const float flashTime = phaseTimer - CLEAR_PAUSE;
            bool white = false;

            if (flashTime >= 0.0f) {
                const int step = static_cast<int>(flashTime / FLASH_INTERVAL);
                white = (step % 2 == 0);
            }
            renderer.setMazeFlash(white);

            if (flashTime >= 2 * FLASH_COUNT * FLASH_INTERVAL)
                startNextRound();
        }

        renderer.render(pacman, gameState, fruitSpawner, maze);
    }
}
