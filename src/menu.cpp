#include "menu.hpp"

// Construtor da classe Menu
// Padrão, não precisa de nenhum parametro a mais
Menu::Menu() : selectedOption(0), currentState(MAIN_MENU){

    // Carrega a fonte usada em todos os textos do menu
    font.loadFromFile("assets/fonts/qager.ttf");

    // Configuração do quadro de líderes
    leaderboardText.clear(); // Limpa o vetor antes de adicionar novos elementos
    
    // Adiciona 5 textos padrão ao vetor como placeholders
    for(int i = 0; i < 5; ++i){
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition(50, 250 + i * 40); // Posiciona verticalmente
        leaderboardText.push_back(text);
    }

    // Configuração do título "Quadro de Líderes"
    leaderboardTitle.setFont(font);
    leaderboardTitle.setString("Quadro de Lideres");
    leaderboardTitle.setCharacterSize(30);
    leaderboardTitle.setFillColor(sf::Color::Yellow);
    leaderboardTitle.setStyle(sf::Text::Bold);
    leaderboardTitle.setPosition(400 - 110, 300); // Posição centralizada abaixo da imagem


    // Carrega as imagens de fundo
    backgroundTextureMain.loadFromFile("assets/images/menu.png");
    backgroundTextureRules.loadFromFile("assets/images/regras.png");
    backgroundTextureLeaderboard.loadFromFile("assets/images/quadroLideres.png");
    gameOverTexture.loadFromFile("assets/images/gameover.png");

    // Configuração do Sprite de Game Over
    gameOverSprite.setTexture(gameOverTexture);
    float scaleX = 0.5f; // Reduz a largura da imagem para 50%
    float scaleY = 0.5f; // Reduz a altura da imagem para 50%
    gameOverSprite.setScale(scaleX, scaleY);
    float posX = 400 - (gameOverTexture.getSize().x * scaleX) / 2; // Centraliza horizontalmente
    float posY = 400 - (gameOverTexture.getSize().y * scaleY) / 2; // Centraliza verticalmente
    gameOverSprite.setPosition(posX, posY);

    // Configuração dos Sprites de fundo para cada tela
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
    rulesTitle.setString("Rules");
    rulesTitle.setCharacterSize(30);
    rulesTitle.setFillColor(sf::Color::Yellow);
    rulesTitle.setStyle(sf::Text::Bold);
    rulesTitle.setPosition(400 - 50, 300); // Posição centralizada abaixo da imagem

    // Configuração do título "Quadro de Líderes"
    leaderboardTitle.setFont(font);
    leaderboardTitle.setString("Leaderboard");
    leaderboardTitle.setCharacterSize(30);
    leaderboardTitle.setFillColor(sf::Color::Yellow);
    leaderboardTitle.setStyle(sf::Text::Bold);
    leaderboardTitle.setPosition(400 - 110, 300); // Posição centralizada abaixo da imagem

    // Configuração do texto de regras
    rulesText.setFont(font);
    rulesText.setString("-Use the arrow keys to move.\n"
        "-Eat the fruits to score points.\n"
        "-Avoid the ghosts!"
        "-Score as many points as possible to enter the leaderboard"
        "-Temporarily eliminate the ghosts when you eat the special fruit.\n");
    rulesText.setCharacterSize(20);
    rulesText.setFillColor(sf::Color::White);
    rulesText.setPosition(50, 250);

    // Botão voltar
    backButton.setFont(font);
    backButton.setString("Press Enter to Return");
    backButton.setCharacterSize(30);
    backButton.setFillColor(sf::Color::Yellow);
    backButton.setPosition(50, 500);

    // Opções do menu principal
    std::vector<std::string> optionTexts = {"      Play", "     Rules", "Leaderboard", "       Quit"};
    for(size_t i = 0; i < optionTexts.size(); i++){
        sf::Text text;
        initMenuText(text, optionTexts[i], 300, 300 + i * 100);
        options.push_back(text);
    }
}

// Método para configurar o quadro de líderes
void Menu::setLeaderboard(const std::vector<ScoreEntry>& scores){
    leaderboardText.clear(); // Limpa o vetor existente de textos do leaderboard

    for(size_t i = 0; i < scores.size(); ++i){
        sf::Text text;
        text.setFont(font); // Define a fonte do texto
        text.setCharacterSize(20); // Tamanho da fonte
        text.setFillColor(sf::Color::White); // Cor branca padrão

        // Monta a string no formato "1. Nome - Pontuação"
        text.setString(std::to_string(i + 1) + ". " + scores[i].name + " - " + std::to_string(scores[i].score));
        
        // Posiciona o texto verticalmente no quadro de líderes
        text.setPosition(50, 350 + i * 40);
        
        // Adiciona o texto ao vetor
        leaderboardText.push_back(text); 
    }
}

// Método auxiliar para inicializar os textos do menu
void Menu::initMenuText(sf::Text& text, const std::string& str, float x, float y){
    text.setFont(font); // Define a fonte do texto
    text.setString(str); // Define o conteúdo da string
    text.setCharacterSize(40); // Tamanho da fonte
    text.setFillColor(sf::Color::White); // Cor padrão branca
    text.setPosition(x, y); // Posição na tela
}

// Método para desenhar os elementos da interface
void Menu::draw(sf::RenderWindow& window){
    window.clear(sf::Color(10, 10, 50)); // Fundo azul escuro

    // Tela do Menu Principal
    if(this->currentState == MAIN_MENU){
        window.draw(backgroundSpriteMain); // Desenha a imagem de fundo do menu

        // Desenha as opções do menu e destaca a opção selecionada
        for(size_t i = 0; i < options.size(); i++){
            options[i].setFillColor(static_cast<int>(i) == selectedOption ? sf::Color::Red : sf::Color::White);
            window.draw(options[i]);
        }
    } 
    
    // Tela de Regras
    else if(this->currentState == RULES_SCREEN){
        window.draw(backgroundSpriteRules); // Desenha o fundo da tela de regras
        window.draw(rulesTitle); // Desenha o título "Rules"
        window.draw(rulesText);  // Desenha as instruções
        window.draw(backButton); // Desenha o botão "Voltar"
    } 
    
    // Tela do Quadro de Líderes
    else if(this->currentState == LEADERBOARD_SCREEN){
        window.draw(backgroundSpriteLeaderboard);// Desenha o fundo da tela de líderes
        window.draw(leaderboardTitle); // Desenha o título "Leaderboard"

         // Desenha cada linha do quadro
        for(const auto& text : leaderboardText)
            window.draw(text);
        
        window.draw(backButton); // Desenha o botão "Voltar"
    }
}

// Método para atualizar a opção selecionada no menu
void Menu::updateSelection(int direction){
    // Ajusta a opção selecionada ao pressionar as setas para cima/baixo
    selectedOption = (selectedOption + direction + options.size()) % options.size();
}

// Método para lidar com as entradas do teclado
int Menu::handleInput(const sf::Event& event){
    if(event.type == sf::Event::KeyPressed){
        
        // Se estiver no menu principal
        if(this->currentState == MAIN_MENU){
            if(event.key.code == sf::Keyboard::Up) 
                updateSelection(-1); // Move a seleção para cima

            else if(event.key.code == sf::Keyboard::Down) 
                updateSelection(1); // Move a seleção para baixo
            
            else if(event.key.code == sf::Keyboard::Enter)
                return selectedOption; // Retorna a opção selecionada
            
        } 

        // Se estiver em outra tela (Regras ou Leaderboard)
        else{
            if(event.key.code == sf::Keyboard::Enter) 
                this->currentState = MAIN_MENU; // Voltar ao menu principal
        }
    }
    return -1; // Nenhuma opção selecionada
}

// Método para definir o estado atual do menu
void Menu::setState(MenuState state){
    this->currentState = state;
}
