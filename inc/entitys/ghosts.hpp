#ifndef GHOSTS_HPP
#define GHOSTS_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Ghost{
    private:
        sf::Sprite sprite;       // Sprite para representar o fantasma visualmente
        sf::Texture texture;     // Textura para carregar a imagem do fantasma
        sf::Vector2f pos;        // Posição atual (x, y)
        sf::Vector2f dir;        // Direção atual (-1, 0, ou 1 para x e y)

        float speed;             // Velocidade de movimento
        
        bool inGame;
        bool powerless;
        int dificult;
};

#endif