#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.hpp"
#include "entitys/pacman.hpp"

//#include "entitys.hpp"

int main(){
sf::RenderWindow window(sf::VideoMode(12000, 6000), "Pac-Man");
    Pacman pacman("assets/images/pacman.png", {100.f, 100.f}, 200.f); // Posição inicial e velocidade
    Map gameMap({10.f, 10.f}); // Cada célula do mapa tem 40x40 pixels

    if (!gameMap.loadFromFile("assets/maps/map.txt")) {
        return 1; // Sai se o arquivo do mapa não for carregado
    }

    sf::Clock clock; // Para medir o delta time

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Controle do Pacman
        sf::Vector2f direction(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            direction.y = -1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            direction.y = 1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            direction.x = -1;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            direction.x = 1;
        }
        pacman.setDirection(direction);

        // Atualização do jogo
        float deltaTime = clock.restart().asSeconds();
        pacman.update(deltaTime);

        // Desenho
        window.clear();
        gameMap.draw(window); // Desenha o mapa
        //pacman.draw(window);  // Desenha o Pacman
        window.display();
    }

    return 0;
}

// https://downloads.khinsider.com/game-soundtracks/album/pac-man-game-sound-effects