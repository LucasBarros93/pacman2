#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "entitys/pacman.hpp"
#include "entitys/ghosts.hpp"
#include "entitys/fruits.hpp"

typedef std::vector<std::vector <char>> MapData;

class Map{
    private:
        MapData mapData;  // Representação do mapa
        sf::Vector2<float> tileSize;            // Tamanho de cada célula do mapa

        sf::RectangleShape wall;            // Sprite para paredes
        sf::CircleShape dot;      // Forma para os pontos
        
        Pacman pac;
        Blinky blinky;
        Pinky pinky;
        Inky inky;
        Clyde clyde;

        // Vetores para armazenar frutas
        std::map<std::pair<int, int>, std::unique_ptr<Fruit>> fruits;
        
    public:
        Map(const sf::Vector2<float>& tileSize);
    
        bool loadFromFile(const std::string& filePath); // Carrega o mapa de um arquivo
        void draw(sf::RenderWindow& window);           // Desenha o mapa na janela
        const MapData& getMapData() const; // Retorna os dados do mapa

        void updatePacman(const sf::Vector2<int> direction);
        void updateGhosts();

        void colision();

};

#endif