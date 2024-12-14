#include "menu.hpp"

Menu::Menu() : selectedOption(0) {
    // Carrega a fonte
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        throw std::runtime_error("Erro ao carregar a fonte!");
    }

    // Título
    title.setFont(font);
    title.setString("PAC-MAN");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(300, 100);

    // Opções do menu
    std::vector<std::string> optionTexts = {"Jogar", "Regras", "Quadro de Lideres", "Sair"};

    for (size_t i = 0; i < optionTexts.size(); i++) {
        sf::Text text;
        initMenuText(text, optionTexts[i], 350, 200 + i * 100);
        options.push_back(text);
    }
}

void Menu::initMenuText(sf::Text& text, const std::string& str, float x, float y) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

void Menu::draw(sf::RenderWindow& window) {
    window.draw(title);

    for (size_t i = 0; i < options.size(); i++) {
        options[i].setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
        window.draw(options[i]);
    }
}

void Menu::updateSelection(int direction) {
    selectedOption = (selectedOption + direction + options.size()) % options.size();
}

int Menu::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            updateSelection(-1);
        } else if (event.key.code == sf::Keyboard::Down) {
            updateSelection(1);
        } else if (event.key.code == sf::Keyboard::Enter) {
            return selectedOption;
        }
    }
    return -1; // Nenhuma opção selecionada
}
