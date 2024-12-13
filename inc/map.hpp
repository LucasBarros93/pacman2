#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "entitys/pacman.hpp"

class Map{
    private:
        std::vector<std::string> mapData;  // Representação do mapa
        // sf::Texture wallTexture;          // Textura para paredes
        sf::RectangleShape wall;            // Sprite para paredes
        sf::CircleShape dot;      // Forma para os pontos
        sf::Vector2f tileSize;            // Tamanho de cada célula do mapa
        
        Pacman pac;
        
    public:
        Map(const sf::Vector2f& tileSize);
    
        bool loadFromFile(const std::string& filePath); // Carrega o mapa de um arquivo
        void draw(sf::RenderWindow& window);           // Desenha o mapa na janela
        const std::vector<std::string>& getMapData() const; // Retorna os dados do mapa

};

#endif