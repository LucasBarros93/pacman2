#ifndef FRUITS_HPP
#define FRUITS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <memory>

// ==========================
// Classe base abstrata Fruit
// ==========================
// Essa classe define um padrao comum pra todas as frutas
// Essa classe serve como base polimorfica pras demais
class Fruit{
    public:
        virtual ~Fruit() = default;  
        virtual int getPoints() const = 0; // Metodo virtual pra retornar os pontos da fruta
        virtual void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const = 0; // Metodo tambem virtual para desenhar a fruta na tela
};

// ==========================
// Classe Dot (fruta comum, a que mais tem no mapa)
// ==========================
class Dot : public Fruit{
    private:
        sf::Vector2<int> pos; // Posicao da fruta no mapa
        int points; // Pontos que o jogador ganha quando pega a fruta

    public:
        Dot() : pos(0, 0), points(10){}; // Construtor padrao com valor inicial
        Dot(sf::Vector2<int> position); // Construtor que inicia a posicao
        int getPoints() const override; // Retorna os pontos da fruta
        void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const override; // Desenha a fruta
};

// ==========================
// Classe Energizer (ponto maior)
// ==========================
// Representa as frutas que dao poder ao pacman de eliminar os fantasmas
class Energizer : public Fruit{
    private:
        sf::Vector2<int> pos; // Posicao da fruta no grid
        int points; // Pontos que o jogador ganha quando pega a fruta

    public:
        Energizer() : pos(0, 0), points(50){}; // Construtor padrao com valor inicial
        Energizer(sf::Vector2<int> position); // Construtor que inicia a posicao
        int getPoints() const override; // Retorna os pontos da fruta
        void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const override; // Desenha a fruta
};

// ==========================
// Classe Bonus (desenho especial, fruta de verdade ~cereja etc)
// ==========================
// Representa as frutas bonus que aparecem as vezes no mapa e dao muitos pontos
class Bonus : public Fruit{
    private:
        sf::Sprite sprite; // Sprite que representa a fruta na tela
        sf::Texture texture; // Textura do sprite
        sf::Vector2<int> pos; // Posicao no mapa
        int points; // Pontos que o jogador ganha quando pega a fruta
        sf::IntRect frame; // Retangulo pra recortar o sprite
        // Aqui abaixo a largura e altura da fruta na imagem do sprite (png)
        const int frameWidth;
        const int frameHeight;
        bool active; // Diz se a fruta esta ativa ou nao no mapa (visibilidade da fruta)
        sf::Clock visibilityTimer; // Timer pra contar o tempo que a fruta fica disponivel/visivel na tela
        sf::Clock respawnTimer;    // Timer pra contar o tempo que a fruta fica invisivel na tela
        void randomize(); // Deixa aleatoria a posicao da fruta e sua aparencia

    public:
        Bonus(int fw, int fh); // Construtor que inicializa as dimensoes do sprite png
        void spawn(const std::vector<std::vector<char>>& mapData); // Gera uma posicao aleatoria de spawn para a fruta bonus (posicao valida)
        void update(const std::vector<std::vector<char>>& mapData);  // Atualiza o status da fruta (ativa ou inativa)
        int getPoints() const override; // Retorna os pontos da fruta
        void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const override; // Desenha a fruta  
        void reset(); // Reseta a fruta bonus, volta ela pro estado inicial
        bool isActive() const; // Apenas retorna se a fruta esta ativa ou nao
        sf::Vector2<int> getPosition() const; // Retorna a posicao da fruta no grid
};


#endif // FRUITS_HPP