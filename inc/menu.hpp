#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <score_manager.hpp>

// 'Enum' pra definir os possiveis estados do menu
enum MenuState{MAIN_MENU, RULES_SCREEN, LEADERBOARD_SCREEN}; 


// =====================================
// Classe Menu
// =====================================
// Lida com o menu principal, regras, quadro de lideres e a exibicao de "game over"
class Menu{
    private:
        sf::Font font; // Fonte para o texto
        sf::Text title; // Titulo do menu
        std::vector<sf::Text> options; // Opcoes do menu
        int selectedOption; // Nro. da opcao selecionada
        MenuState currentState; // Estado atual do menu
        sf::Text rulesTitle; // Titulo para a tela de regras
        sf::Text leaderboardTitle; // Titulo para o quadro de lideres
        sf::Text rulesText; // Conteudo do texto que vai nas regras
        std::vector<sf::Text> leaderboardText; // Conteudo do texto que vai no quadro de lideres 
        sf::Text backButton; // Opcao de voltar pro menu principal,nesse caso quando o jogo ja iniciou nao tem como voltar. a tecla pra voltar eh enter nos casos em que eh permitido
        // Texturas e sprites para diferentes lugares
        sf::Texture backgroundTextureMain;// Textura do fundo do menu principal
        sf::Sprite backgroundSpriteMain; // sprite pra desenhar o fundo do menu principal
        sf::Texture backgroundTextureRules; // Textura do fundo da tela das regras
        sf::Sprite backgroundSpriteRules; //Sprite pra desenhar o fundo da tela de regras
        sf::Texture backgroundTextureLeaderboard; // Textura do fundo do quadro de lideres
        sf::Sprite backgroundSpriteLeaderboard; // Sprite pra desenhar o quadro de lideres
        sf::Texture gameOverTexture;    // Textura pra imagem de game over
        sf::Sprite gameOverSprite;      // Sprite pra exibir a imagem
        void initMenuText(sf::Text& text, const std::string& str, float x, float y); // inicia um texto e define as propriedades dele tbm

    public:
        Menu();
        void draw(sf::RenderWindow& window); // Desenha o menu e a janela do que for (regras e qdro de lider tbm)
        int handleInput(const sf::Event& event); // Lida com as entradas do usuario, como enter e setinhas
        void updateSelection(int direction); // Atualiza a opcao selecionada no menu
        void setLeaderboard(const std::vector<ScoreEntry>& scores); // Configura o quadro de lideres de acordo com as pontuacoes jogadas pelo publico
        void setState(MenuState state); // Define o estado atual do menu MAIN_MENU, RULES_SCREEN...
};

#endif