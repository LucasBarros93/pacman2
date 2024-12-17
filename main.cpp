#include <SFML/Graphics.hpp>
#include "menu.hpp"
#include "map.hpp"
#include "score_manager.hpp"
#include "score.hpp"

int main(){

    // Cria uma janela com resolução 800x800 e título "PacMan"
    sf::RenderWindow window(sf::VideoMode(800, 800), "PacMan");

    int result; // Variável para armazenar resultados de pontos ganhos

    // Instancia os componentes do jogo
    Menu menu; // Menu principal do jogo
    Map gameMap({10.f, 10.f}); // Mapa do jogo, com tamanho dos "tiles" = 10,10
    ScoreManager scoreManager("./assets/leaderboard/scores.txt"); // Gerenciador de pontuações

    // Variáveis relacionadas ao jogador
    std::string playerName; // Nome do jogador
    Score playerScore(0); // Pontuação inicial do jogador
    int currentLvl = 1; // Fase do jogo

    // Flags de controle do jogo
    bool inGame = false, enteringName = false, gameOver = false;

    // Carrega a fonte
    sf::Font font;
    font.loadFromFile("assets/fonts/qager.ttf");

    // Carrega a imagem de Game Over
    sf::Texture gameOverTexture;
    gameOverTexture.loadFromFile("assets/images/gameover.png");
    sf::Sprite gameOverSprite;
    gameOverSprite.setTexture(gameOverTexture);

    // Centraliza a imagem de Game Over
    float scaleX = 0.5f; // Reduz a largura para 50%
    float scaleY = 0.5f; // Reduz a altura para 50%
    gameOverSprite.setScale(scaleX, scaleY);
    gameOverSprite.setPosition(
        // Centraliza
        390 - (gameOverTexture.getSize().x * scaleX)/ 2,
        400 - (gameOverTexture.getSize().y * scaleY)/ 2
    );

    // Texto de entrada do jogador
    sf::Text inputText, gameOverText, scoreText, phaseText;
    inputText.setFont(font);
    inputText.setCharacterSize(30);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(100, 200);
    
    // Configuração do texto de Game Over
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setString("Pressione Enter para voltar ao menu");
    gameOverText.setPosition(120, 650);

    // Captura o tamanho atual da janela
    sf::Vector2u winSize = window.getSize(); 

    // Configuração do texto de pontuação (Score)
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Yellow);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition( (winSize.x * 0.125f), (winSize.y * 0.875f));   // Por exemplo, 12.5% da largura, 87.5% da altura

    // Configuração do texto de fase atual (Phase)
    phaseText.setFont(font);
    phaseText.setCharacterSize(24);
    phaseText.setFillColor(sf::Color::Yellow);
    phaseText.setStyle(sf::Text::Bold);
    phaseText.setPosition( (winSize.x * 0.5f),   (winSize.y * 0.875f));   // 50% da largura, 87.5% da altura

    // Relógio para controlar o tempo de atualização do jogo
    sf::Clock clock;
    float updateTime = 0.1f; // Intervalo de atualização em segundos
    float elapsedTime = 0.0f; // Tempo acumulado

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            // Fecha a janela quando o evento de fechar é acionado
            if(event.type == sf::Event::Closed)
                window.close();

            // Menu principal: trata a entrada do jogador
            if(!inGame && !enteringName && !gameOver){
                int option = menu.handleInput(event); // Captura a opção selecionada no menu

                if(option == 0){// Opção "Iniciar Jogo" selecionada
                    enteringName = true; 
                    playerName.clear(); // Limpa o nome do jogador
                }

                else if(option == 1)// Opção "Regras do Jogo"
                    menu.setState(RULES_SCREEN);
                
                else if(option == 2){ // Opção "Placar"
                    auto scores = scoreManager.getScores();
                    std::vector<ScoreEntry> leaderboard;

                    // Converte o vetor de pares em ScoreEntry
                    for (const auto& score : scores)
                        leaderboard.emplace_back(score.first, score.second);
                    
                    menu.setLeaderboard(leaderboard); 
                    menu.setState(LEADERBOARD_SCREEN);
                }

                else if(option == 3)// Opção "Sair do Jogo"
                    window.close();
            }

            // Entrada do nome do jogador
            else if(enteringName && event.type == sf::Event::TextEntered){
                
                if(event.text.unicode == '\b' && !playerName.empty())
                    playerName.pop_back(); // Remove o último caractere

                // Enter pressionado, inicia o jogo
                else if(event.text.unicode == '\r' && !playerName.empty()){
                    enteringName = false;
                    inGame = true;
                    playerScore = 0; // Reseta o score
                    currentLvl = 1; // Fase inicial
                    gameMap.loadFromFile("assets/maps/map.txt"); // Carrega o mapa

                }
                // Digitando nome do jogador
                else if(event.text.unicode < 128 && playerName.size()< 15)
                    playerName += static_cast<char>(event.text.unicode); // Adiciona o caractere digitado
                
            } 

            // Game Over
            else if(gameOver){
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter){
                    // Reseta o jogo ao pressionar Enter na tela de Game Over
                    inGame = false;
                    scoreManager.saveScore(playerName, playerScore.getValue());  // Salva a pontuação
                    enteringName = false;
                    gameOver = false;
                    playerName.clear(); // Limpa o nome do jogador
                    playerScore = 0;  // Reseta o pontuação
                    currentLvl = 1; // Reseta a level
                    gameMap.reset();  // Reseta o mapa e as entidades
                    menu.setState(MAIN_MENU); // Volta ao menu principal
                }
            }
        }

        // Fundo azul escuro em todas as telas
        window.clear(sf::Color(10, 10, 50));

        // Tela de entrada do nome do jogador
        if(enteringName){
            inputText.setString("Enter your name: " + playerName);
            window.draw(inputText);
        } 
        // Tela de Game Over
        else if(gameOver){
            window.draw(gameOverSprite); // Desenha a imagem de Game Over
            window.draw(gameOverText);  // Texto de Game Over
        } 
        // Menu principal
        else if(!inGame){
            menu.draw(window); // Desenha o menu
        } 
        // Lógica do jogo em execução
        else{
            float deltaTime = clock.restart().asSeconds(); // Tempo decorrido desde o último frame
            elapsedTime += deltaTime;

            // Atualiza o jogo em intervalos fixos
            if(elapsedTime >= updateTime){
                sf::Vector2<int> direction(0, 0);

                // Captura a direção do movimento com base nas teclas pressionadas
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))direction.y = -1;
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))direction.y = 1;
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))direction.x = -1;
                else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))direction.x = 1;

                // Atualiza a posição do PacMan
                gameMap.updatePacman(direction);

                // Verifica colisões (pontuação ou GameOver)
                result = gameMap.colision();
                playerScore += result>0? result : 0;
                gameOver = result == -1? true : false;

                // Atualiza os fantasmas e frutas bônus no mapa
                gameMap.updateGhosts();
                gameMap.updateBonusFruit();
                gameMap++;
                elapsedTime = 0.0f; // Reseta o tempo decorrido
            }

            // Verifica se todas as frutas foram coletadas para avançar de fase
            if(gameMap.getFruitsRemaining()== 0){
                currentLvl++;
                gameMap.setLevel(currentLvl); // Atualiza o level no mapa, jogo vai ficar mais dificil
                playerScore += 100 * currentLvl; // Bonus de pontuação ao mudar de fase
                gameMap.reset(); // Reseta fantasmas e objetos no mapa
            }

            // Desenha o mapa e a interface
            gameMap.draw(window);
            scoreText.setString("Score: " + std::to_string(playerScore.getValue()));
            phaseText.setString("Level: " + std::to_string(currentLvl));

            window.draw(scoreText); // Exibe a pontuação
            window.draw(phaseText); // Exibe o nível atual
        }

        window.display(); // Atualiza a tela
    }

    return 0;
}