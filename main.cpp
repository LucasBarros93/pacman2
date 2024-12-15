#include <SFML/Graphics.hpp>
#include "menu.hpp"
#include "map.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Pac-Man Menu");

    Menu menu;
    Map gameMap({10.f, 10.f});
    bool inGame = false;

    sf::Clock clock;  // Relógio para medir delta time
    const float updateTime = 0.1f;  // Tempo mínimo entre movimentos (em segundos)
    float elapsedTime = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!inGame) {
                int option = menu.handleInput(event);
                if (option == 0) { // Jogar
                    inGame = true;
                    gameMap.loadFromFile("assets/maps/map.txt");
                } else if (option == 1) { // Regras
                    std::cout << "Regras do jogo: \n- Movimente o Pac-Man usando as setas.\n- Coma os pontos e evite os fantasmas!\n";
                } else if (option == 2) { // Quadro de líderes
                    std::cout << "Quadro de líderes ainda não implementado!\n";
                } else if (option == 3) { // Sair
                    window.close();
                }
            }
        }

        // Calcula o tempo decorrido desde o último quadro
        float deltaTime = clock.restart().asSeconds();
        elapsedTime += deltaTime;

        window.clear();

        if (!inGame) {
            menu.draw(window);
        } else {
            // Atualiza o movimento do Pac-Man somente após atingir o tempo limite
            if (elapsedTime >= updateTime) {
                sf::Vector2<int> direction(0, 0);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    direction.y = -1;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    direction.y = 1;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    direction.x = -1;
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    direction.x = 1;
                }

                gameMap.updatePacman(direction);
                gameMap.updateGhosts();
                elapsedTime = 0.0f;  // Reinicia o timer
            }

            // Desenha o jogo
            gameMap.draw(window);
        }

        window.display();
    }

    return 0;
}
