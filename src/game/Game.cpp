#include "game/Game.hpp"

Game::Game()
    : window(sf::VideoMode({800, 600}), "Pacman"), pacman({13.5f, 23.0f}),
      renderer(window), maze("./assets/maps/map.txt") {}

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

    while (this->window.isOpen()) {

        float deltaTime = clock.restart().asSeconds();

        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        Direction newDirection = this->getInput();
        pacman.trySetDirection(newDirection, this->maze);
        pacman.update(this->maze, deltaTime);

        window.clear();

        renderer.renderMaze(maze);
        renderer.renderPacman(pacman);

        window.display();
    }
}
