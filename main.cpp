#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.hpp"

//#include "entitys.hpp"

int main(){
    sf::RenderWindow window(sf::VideoMode(12000, 6000), "Pacman");
    Map gameMap("assets/images/wall.png", {20.f, 20.f}); // Cada célula tem 40x40 pixels

    if (!gameMap.loadFromFile("assets/maps/map.txt")) {
        return 1; // Sai se o arquivo do mapa não for carregado
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        gameMap.draw(window); // Desenha o mapa
        window.display();
    }

    return 0;
}

// https://downloads.khinsider.com/game-soundtracks/album/pac-man-game-sound-effects