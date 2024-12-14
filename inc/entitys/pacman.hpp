#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Pacman {
    private:
        sf::Sprite sprite;       // Sprite para representar o Pacman visualmente
        sf::Texture texture;     // Textura para carregar a imagem do Pacman
        sf::Vector2<int> pos;        // Posição atual (x, y)
        sf::Vector2<int> dir;        // Direção atual (-1, 0, ou 1 para x e y)

        sf::IntRect currentFrame;
        sf::Clock animationClock;

        const int frameWidth;      // Largura do frame (ex: 16 pixels)
        const int frameHeight;     // Altura do frame (ex: 16 pixels)
        const int frameCount;      // Quantidade de frames por direção
        float frameDuration;       // Tempo entre frames
        int currentFrameIndex;     // Índice do frame atual

    public:
        Pacman(const std::string& texturePath, int fw, int fh, float fd);

        void updateAnimation();
        std::vector<std::vector <char>> update(std::vector<std::vector <char>> mapData, const sf::Vector2<int> direction);

        void setDirection(const sf::Vector2<int>& direction); // Define a direção de movimento
        void setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize);

        const sf::Sprite getSprite() const;             // Desenha o Pacman na janela
        const sf::Vector2<int>& getPosition() const;         // Retorna a posição atual
        const sf::Vector2<int>& getDirection() const;        // Retorna a direção atual
};

#endif // PACMAN_HPP
