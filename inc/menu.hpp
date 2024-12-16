#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <score_manager.hpp>

enum MenuState {
    MAIN_MENU,
    RULES_SCREEN,
    LEADERBOARD_SCREEN
};

class Menu {
private:
    sf::Font font; // Fonte para o texto
    sf::Text title; // Título do menu
    std::vector<sf::Text> options; // Opções do menu
    int selectedOption; // Índice da opção selecionada
    MenuState currentState; // Estado atual do menu

    sf::Text rulesText; // Texto para as regras
    sf::Text leaderboardText; // Texto para o quadro de líderes
    sf::Text backButton; // Botão de voltar ao menu principal

    void initMenuText(sf::Text& text, const std::string& str, float x, float y);

public:
    Menu();

    void draw(sf::RenderWindow& window); // Desenha o menu
    int handleInput(const sf::Event& event); // Lida com entradas do usuário
    void updateSelection(int direction); // Atualiza a opção selecionada
    void setLeaderboard(const std::vector<ScoreEntry>& scores);
    void setState(MenuState state); // Define o estado atual do menu
};

#endif
