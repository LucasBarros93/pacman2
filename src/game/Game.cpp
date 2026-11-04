#include "game/Game.hpp"

namespace {
enum class RoundPhase { Playing, Clearing };

constexpr float CLEAR_PAUSE = 0.8f;
constexpr float FLASH_INTERVAL = 0.25f;
constexpr int FLASH_COUNT = 3;
} // namespace

Game::Game()
    : window(sf::VideoMode({800, 600}), "Pacman"), gameState(),
      fruitSpawner({13.5f, 17.0f}), maze("./assets/maps/map.txt"),
      pacman({13.5f, 23.0f}), renderer(window) {}

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

    RoundPhase phase = RoundPhase::Playing;
    float phaseTimer = 0.0f;

    auto startNextRound = [&]() {
        gameState.advanceLevel();
        maze.reset();
        pacman.reset();
        fruitSpawner.resetRound();
        dotsEatenThisRound = 0;
        renderer.setMazeFlash(false);
        phase = RoundPhase::Playing;
    };

    while (this->window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
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
