#include <SFML/Graphics.hpp>
#include <locale>
#include "menu.hpp"
#include "map.hpp"
#include "score_manager.hpp"
#include "score.hpp"

int main() {
    setlocale(LC_ALL, ""); // Suporte a caracteres especiais

    sf::RenderWindow window(sf::VideoMode(800, 800), "Pac-Man");

    int result;

    Menu menu;
    Map gameMap({10.f, 10.f});
    ScoreManager scoreManager("./assets/leaderboard/scores.txt");

    std::string playerName;
    Score playerScore(0);
    int currentPhase = 1;

    bool inGame = false, enteringName = false, gameOver = false;

    // Carrega a fonte
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/qager.ttf")) {
        throw std::runtime_error("Erro ao carregar a fonte!");
    }

    // Carrega a imagem de Game Over
    sf::Texture gameOverTexture;
    if (!gameOverTexture.loadFromFile("assets/images/gameover.png")) {
        throw std::runtime_error("Erro ao carregar a imagem de Game Over!");
    }
    sf::Sprite gameOverSprite;
    gameOverSprite.setTexture(gameOverTexture);

    // Centraliza a imagem de Game Over
    float scaleX = 0.5f; // Ajuste a escala para caber bem na tela
    float scaleY = 0.5f;
    gameOverSprite.setScale(scaleX, scaleY);
    gameOverSprite.setPosition(
        400 - (gameOverTexture.getSize().x * scaleX) / 2,
        400 - (gameOverTexture.getSize().y * scaleY) / 2
    );

    // Textos da interface
    sf::Text inputText, gameOverText, scoreText, phaseText;
    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(100, 200);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setString("Pressione Enter para voltar ao menu");
    gameOverText.setPosition(200, 650);
    // Ajusta a posição do scoreText e phaseText de acordo com o tamanho da janela
    sf::Vector2u winSize = window.getSize(); 
    scoreText.setPosition( (winSize.x * 0.125f), (winSize.y * 0.875f) );   // Por exemplo, 12.5% da largura, 87.5% da altura
    phaseText.setPosition( (winSize.x * 0.5f),   (winSize.y * 0.875f) );   // 50% da largura, 87.5% da altura

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setStyle(sf::Text::Bold);
    //scoreText.setPosition(100.f, 700.f);

    phaseText.setFont(font);
    phaseText.setCharacterSize(24);
    phaseText.setFillColor(sf::Color::Yellow);
    phaseText.setStyle(sf::Text::Bold);
    //phaseText.setPosition(400.f, 700.f);

    sf::Clock clock;
    float updateTime = 0.1f, elapsedTime = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!inGame && !enteringName && !gameOver) {
                int option = menu.handleInput(event);
                if (option == 0) {
                    enteringName = true; 
                    playerName.clear(); 
                }
                else if (option == 1)
                    menu.setState(RULES_SCREEN);
                else if (option == 2) {
                    // Converte o vetor de pares para ScoreEntry antes de enviar
                    auto scores = scoreManager.getScores();
                    std::vector<ScoreEntry> leaderboard;
                    for (const auto& score : scores) {
                        leaderboard.emplace_back(score.first, score.second);
                    }
                    menu.setLeaderboard(leaderboard); 
                    menu.setState(LEADERBOARD_SCREEN);
                }
                else if (option == 3) 
                    window.close();
            }
            else if (enteringName && event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !playerName.empty())
                    playerName.pop_back();

                else if (event.text.unicode == '\r' && !playerName.empty()) {
                    enteringName = false;
                    inGame = true;
                    playerScore = 0; 
                    currentPhase = 1;
                    gameMap.loadFromFile("assets/maps/map.txt");

                } else if (event.text.unicode < 128 && playerName.size() < 15)
                    playerName += static_cast<char>(event.text.unicode);
                
            } else if (gameOver) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    inGame = false;
                    scoreManager.saveScore(playerName, playerScore.getValue());  // Salva a pontuação
                    enteringName = false;
                    gameOver = false;
                    playerName.clear();
                    playerScore = 0;  // Reseta o score
                    currentPhase = 1; // Reseta a fase
                    gameMap.reset();  // Reseta o mapa e as entidades
                    menu.setState(MAIN_MENU);
                }
            }
        }

        window.clear(sf::Color(10, 10, 50)); // Fundo azul escuro em todas as telas

        if (enteringName) {
            inputText.setString("Enter your name: " + playerName);
            window.draw(inputText);
        } else if (gameOver) {
            window.draw(gameOverSprite); // Desenha a imagem de Game Over
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

                gameMap.updatePacman(direction);
                result = gameMap.colision();
                playerScore += result>0? result : 0;
                gameOver = result == -1? true : false;
                gameMap.updateGhosts();
                gameMap.updateBonusFruit(); // Atualiza fruta bônus
                gameMap++;
                elapsedTime = 0.0f;
            }

            if (gameMap.getFruitsRemaining() == 0) {
                currentPhase++;
                gameMap.setLevel(currentPhase); // atualiza o level no mapa, jogo vai ficar mais dificil
                playerScore += 100 * currentPhase;
                gameMap.loadFromFile("assets/maps/map.txt");
                gameMap.resetGhosts(); // resetar os fantasmas e suas contagens para sairem de suas "casas"
            }

            gameMap.draw(window);

            scoreText.setString("Score: " + std::to_string(playerScore.getValue()));
            phaseText.setString("Level: " + std::to_string(currentPhase));
            window.draw(scoreText);
            window.draw(phaseText);
        }

        window.display();
    }

    return 0;
}