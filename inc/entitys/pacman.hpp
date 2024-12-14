#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Pacman {
    private:
        sf::Sprite sprite;       // Sprite para representar o Pacman visualmente
        sf::Texture texture;     // Textura para carregar a imagem do Pacman
        sf::Vector2f pos;        // Posição atual (x, y)
        sf::Vector2f dir;        // Direção atual (-1, 0, ou 1 para x e y)

        float speed;             // Velocidade de movimento

    public:
        Pacman();
        Pacman(const std::string& texturePath, const sf::Vector2f& startPos, float speed);

        void setDirection(const sf::Vector2f& direction); // Define a direção de movimento
        void setPosition(const sf::Vector2f& position);
        void update(float deltaTime);                    // Atualiza a posição do Pacman
        const sf::Sprite getSprite() const;             // Desenha o Pacman na janela
        const sf::Vector2f& getPosition() const;         // Retorna a posição atual
        const sf::Vector2f& getDirection() const;        // Retorna a direção atual
};

#endif // PACMAN_HPP
