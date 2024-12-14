#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    sf::Font font; // Fonte para o texto
    sf::Text title; // Título do menu
    std::vector<sf::Text> options; // Opções do menu
    int selectedOption; // Índice da opção selecionada

    void initMenuText(sf::Text& text, const std::string& str, float x, float y);

public:
    Menu();

    void draw(sf::RenderWindow& window); // Desenha o menu
    int handleInput(const sf::Event& event); // Lida com entradas do usuário
    void updateSelection(int direction); // Atualiza a opção selecionada
};

#endif
