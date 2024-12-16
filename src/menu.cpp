#include "menu.hpp"

Menu::Menu() : selectedOption(0), currentState(MAIN_MENU) {
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        throw std::runtime_error("Erro ao carregar a fonte!");
    }

    // Título principal
    title.setFont(font);
    title.setString("PAC-MAN");
    title.setCharacterSize(60);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(300, 100);

    // Opções do menu principal
    std::vector<std::string> optionTexts = {"Jogar", "Regras", "Quadro de Lideres", "Sair"};
    for (size_t i = 0; i < optionTexts.size(); i++) {
        sf::Text text;
        initMenuText(text, optionTexts[i], 350, 200 + i * 100);
        options.push_back(text);
    }

    // Texto para as regras
    rulesText.setFont(font);
    rulesText.setString("Regras:\n- Use as setas para mover.\n- Coma os pontos.\n- Evite os fantasmas!");
    rulesText.setCharacterSize(30);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition(50, 200);

    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(30);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setPosition(50, 200);

    backButton.setFont(font);
    backButton.setString("Pressione Enter para Voltar");
    backButton.setCharacterSize(30);
    backButton.setFillColor(sf::Color::Yellow);
    backButton.setPosition(50, 500);
}

void Menu::setLeaderboard(const std::vector<ScoreEntry>& scores) {
    std::ostringstream stream;
    stream << "Quadro de Lideres:\n";
    for (size_t i = 0; i < scores.size(); ++i) {
        stream << i + 1 << ". " << scores[i].name << " - " << scores[i].score << "\n";
    }
    leaderboardText.setString(stream.str());
}

void Menu::initMenuText(sf::Text& text, const std::string& str, float x, float y) {
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

void Menu::draw(sf::RenderWindow& window) {
    if (currentState == MAIN_MENU) {
        window.draw(title);
        for (size_t i = 0; i < options.size(); i++) {
            options[i].setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
            window.draw(options[i]);
        }
    } else if (currentState == RULES_SCREEN) {
        window.draw(rulesText);
        window.draw(backButton);
    } else if (currentState == LEADERBOARD_SCREEN) {
        window.draw(leaderboardText);
        window.draw(backButton);
    }
}

void Menu::updateSelection(int direction) {
    selectedOption = (selectedOption + direction + options.size()) % options.size();
}

int Menu::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (currentState == MAIN_MENU) {
            if (event.key.code == sf::Keyboard::Up) {
                updateSelection(-1);
            } else if (event.key.code == sf::Keyboard::Down) {
                updateSelection(1);
            } else if (event.key.code == sf::Keyboard::Enter) {
                return selectedOption;
            }
        } else {
            if (event.key.code == sf::Keyboard::Enter) {
                currentState = MAIN_MENU; // Voltar ao menu principal
            }
        }
    }
    return -1;
}

void Menu::setState(MenuState state) {
    currentState = state;
}
