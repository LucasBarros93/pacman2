#include "menu.hpp"

Menu::Menu() : selectedOption(0), currentState(MAIN_MENU){

    font.loadFromFile("assets/fonts/qager.ttf"); //carregando fonte
    // Carrega as imagens de fundo
    backgroundTextureMain.loadFromFile("assets/images/menu.png");
    backgroundTextureRules.loadFromFile("assets/images/regras.png");
    backgroundTextureLeaderboard.loadFromFile("assets/images/quadroLideres.png");
    gameOverTexture.loadFromFile("assets/images/gameover.png");

    // colocando a imagem do gameOver
    gameOverSprite.setTexture(gameOverTexture);
    float scaleX = 0.5f; // Ajuste a escala para caber bem na tela
    float scaleY = 0.5f;
    gameOverSprite.setScale(scaleX, scaleY);
    float posX = 400 - (gameOverTexture.getSize().x * scaleX) / 2;
    float posY = 400 - (gameOverTexture.getSize().y * scaleY) / 2;
    gameOverSprite.setPosition(posX, posY);


    backgroundSpriteMain.setTexture(backgroundTextureMain);
    backgroundSpriteMain.setScale(0.22f, 0.22f);
    backgroundSpriteMain.setPosition(380 - backgroundTextureMain.getSize().x * 0.1f, 90);

    backgroundSpriteRules.setTexture(backgroundTextureRules);
    backgroundSpriteRules.setScale(0.2f, 0.2f);
    backgroundSpriteRules.setPosition(400 - backgroundTextureRules.getSize().x * 0.1f, 50);

    backgroundSpriteLeaderboard.setTexture(backgroundTextureLeaderboard);
    backgroundSpriteLeaderboard.setScale(0.2f, 0.2f);
    backgroundSpriteLeaderboard.setPosition(400 - backgroundTextureLeaderboard.getSize().x * 0.1f, 50);

    // Configuração do título "Regras"
    rulesTitle.setFont(font);
    rulesTitle.setString("Regras");
    rulesTitle.setCharacterSize(30);
    rulesTitle.setFillColor(sf::Color::Yellow);
    rulesTitle.setStyle(sf::Text::Bold);
    rulesTitle.setPosition(400 - 50, 300); // Posição centralizada abaixo da imagem

    // Configuração do título "Quadro de Líderes"
    leaderboardTitle.setFont(font);
    leaderboardTitle.setString("Quadro de Lideres");
    leaderboardTitle.setCharacterSize(30);
    leaderboardTitle.setFillColor(sf::Color::Yellow);
    leaderboardTitle.setStyle(sf::Text::Bold);
    leaderboardTitle.setPosition(400 - 110, 300); // Posição centralizada abaixo da imagem

    // Configuração do texto de regras
    rulesText.setFont(font);
    rulesText.setString("Use as setas para mover\nComa as frutas para pontuar\nEvite os fantasmas!");
    rulesText.setCharacterSize(20);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition(50, 250);

    // Configuração do quadro de líderes
    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(20);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setPosition(50, 250);

    // Botão voltar
    backButton.setFont(font);
    backButton.setString("Pressione Enter para Voltar");
    backButton.setCharacterSize(20);
    backButton.setFillColor(sf::Color::Yellow);
    backButton.setPosition(50, 500);

    // Opções do menu principal
    std::vector<std::string> optionTexts = {"Jogar", "Regras", "Quadro de Lideres", "Sair"};
    for (size_t i = 0; i < optionTexts.size(); i++) {
        sf::Text text;
        initMenuText(text, optionTexts[i], 300, 300 + i * 100);
        options.push_back(text);
    }

    // Texto para as regras
    rulesText.setFont(font);
    rulesText.setString("- Use as setas para mover\n- Pontue comendo as frutas (pontos)\n- Elimine momentaneamente os fantasmas quando comer a fruta especial\n- Pontue o maximo possivel para entrar no quadro de lideres!");
    rulesText.setCharacterSize(15);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition(50, 400);

    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(30);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setPosition(50, 400);

    backButton.setFont(font);
    backButton.setString("Pressione Enter para Voltar");
    backButton.setCharacterSize(30);
    backButton.setFillColor(sf::Color::Yellow);
    backButton.setPosition(50, 700);
}

void Menu::setLeaderboard(const std::vector<ScoreEntry>& scores) {
    std::ostringstream stream;
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
    window.clear(sf::Color(10, 10, 50)); // Fundo azul escuro

    if (currentState == MAIN_MENU) {
        window.draw(backgroundSpriteMain);
        for (size_t i = 0; i < options.size(); i++) {
            options[i].setFillColor(static_cast<int>(i) == selectedOption ? sf::Color::Red : sf::Color::White);
            window.draw(options[i]);
        }
    } else if (currentState == RULES_SCREEN) {
        window.draw(backgroundSpriteRules);
        window.draw(rulesTitle);   // Título "Regras"
        window.draw(rulesText);    // Texto das regras
        window.draw(backButton);
    } else if (currentState == LEADERBOARD_SCREEN) {
        window.draw(backgroundSpriteLeaderboard);
        window.draw(leaderboardTitle);  // Título "Quadro de Líderes"
        window.draw(leaderboardText);   // Texto do quadro de líderes
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
