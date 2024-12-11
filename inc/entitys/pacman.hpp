#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Pacman{
    private:
        sf::Sprite sprite;       // Sprite para representar o Pacman visualmente
        sf::Texture texture;     // Textura para carregar a imagem do Pacman
        sf::Vector2f pos;        // Posição atual (x, y)
        sf::Vector2f dir;        // Direção atual (-1, 0, ou 1 para x e y)

        float speed;             // Velocidade de movimento
};

#endif