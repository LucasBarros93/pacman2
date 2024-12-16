#include <SFML/Graphics.hpp>
#include <locale>
#include "menu.hpp"
#include "map.hpp"
#include "score_manager.hpp"
#include "score.hpp"

int main() {
    setlocale(LC_ALL, ""); // Suporte a caracteres especiais

    sf::RenderWindow window(sf::VideoMode(800, 800), "Pac-Man");

    Menu menu;
    Map gameMap({10.f, 10.f});
    ScoreManager scoreManager("scores.txt");

    std::string playerName;
    Score playerScore(0);
    int currentPhase = 1;

    bool inGame = false, enteringName = false, gameOver = false;

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        throw std::runtime_error("Erro ao carregar a fonte!");
    }

    sf::Text inputText, gameOverText, scoreText, phaseText;
    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(100, 400);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("Fim de Jogo!\nPressione Enter para voltar ao menu.");
    gameOverText.setPosition(100, 350);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(20.f, 10.f); // Coordenada absoluta (20px da esquerda, 10px do topo)

    // Configuração do texto de fase
    phaseText.setFont(font);
    phaseText.setCharacterSize(24);
    phaseText.setFillColor(sf::Color::Yellow);
    phaseText.setStyle(sf::Text::Bold);
    phaseText.setPosition(20.f, 40.f); // Logo abaixo da pontuação

    sf::Clock clock;
    float updateTime = 0.1f, elapsedTime = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (!inGame && !enteringName && !gameOver) {
                int option = menu.handleInput(event);
                if (option == 0) { enteringName = true; playerName.clear(); }
                else if (option == 1) menu.setState(RULES_SCREEN);
                else if (option == 2) {
                    menu.setLeaderboard(scoreManager.getScores());
                    menu.setState(LEADERBOARD_SCREEN);
                } else if (option == 3) window.close();
            } else if (enteringName && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty()) playerName.pop_back();
                else if (event.text.unicode == '\r' && !playerName.empty()) {
                    enteringName = false; inGame = true; playerScore = 0; currentPhase = 1;
                    gameMap.loadFromFile("assets/maps/map.txt");
                } else if (event.text.unicode < 128 && playerName.size() < 15) {
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
        }

        window.clear();

        if (enteringName) {
            inputText.setString("Digite seu nome: " + playerName);
            window.draw(inputText);
        } else if (gameOver) {
            window.draw(gameOverText);
        } else if (!inGame) {
            menu.draw(window);
        } else {
            float deltaTime = clock.restart().asSeconds();
            elapsedTime += deltaTime;

            if (elapsedTime >= updateTime) {
                sf::Vector2<int> direction(0, 0);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) direction.y = -1;
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) direction.y = 1;
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) direction.x = -1;
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) direction.x = 1;

                playerScore += gameMap.updatePacman(direction);
                gameMap.updateGhosts();
                elapsedTime = 0.0f;
            }

            if (gameMap.getFruitsRemaining() == 0) {
                currentPhase++;
                playerScore += 100 * currentPhase;
                gameMap.loadFromFile("assets/maps/map.txt");
            }

            gameMap.draw(window);

            scoreText.setString("Score: " + std::to_string(playerScore.getValue()));
            phaseText.setString("Phase: " + std::to_string(currentPhase));
            window.draw(scoreText);
            window.draw(phaseText);
        }

        window.display();
    }

    return 0;
}