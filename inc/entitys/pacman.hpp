#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>

typedef std::vector<std::vector <char>> MapData; // Tipo de dado para o mapa (representado por uma matriz de caracteres)

// ============================
// Classe Pacman
// ============================
// Classe do personagem principal (PacMan)
// Lida com a animacao, posicao, direcao e interacao com o mapa
class Pacman{
    private:
        sf::Sprite sprite;          // Sprite pra representar o Pacman visualmente
        sf::Texture texture;        // Textura pra carregar a imagem do Pacman
        sf::Vector2<int> pos;       // Posicao atual (x, y)
        sf::Vector2<int> dir;       // Direcao atual (-1, 0, ou 1 para x e y)
        sf::IntRect currentFrame;   // Retangulo que seleciona o frame atual do png
        sf::Clock animationClock;   // Relogio que  controla o tempo entre frames
        const int frameWidth;       // Largura do frame (ex: 16 pixels)
        const int frameHeight;      // Altura do frame (ex: 16 pixels)
        const int frameCount;       // Quantidade de frames por direcao
        float frameDuration;        // Tempo entre frames
        int currentFrameIndex;      // Indice do frame atual

    public:
        Pacman(const std::string& texturePath, int fw, int fh, float fd); // Construtor: Inicia pacman com uma textura, dimensoes e duracao do frame
        void updateAnimation(); // Atualiza a animacao do pacman (troca os frames)
        MapData update(MapData mapData, const sf::Vector2<int> direction); // Atualiza a posicao do pacman no mapa com base na direcao
        void reset(); // reseta dps do gameover
        void setDirection(const sf::Vector2<int>& direction); // Define a direcao de movimento do pac
        void setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize); // Define o lugar inicial do pacman na tela
        const sf::Sprite getSprite() const; // Desenha o Pacman na janela
        const sf::Vector2<int>& getPosition() const; // Retorna a posicao atual dele
        const sf::Vector2<int>& getDirection() const; // Retorna a direcao atual que se movimenta
};

#endif // PACMAN_HPP
