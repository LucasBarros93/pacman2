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
        int count;
        int level; // pega o level que o jogador está atualemnte pra poder 
        sf::Vector2<float> tileSize;            // Tamanho de cada célula do mapa
        sf::Vector2f mapOffset; // Offset para centralizar o mapa na tela
        sf::RectangleShape wall;            // Sprite para paredes
        sf::CircleShape dot;      // Forma para os pontos
        

        Pacman pac;
        Blinky blinky;
        Pinky pinky;
        Inky inky;
        Clyde clyde;

        // Vetores para armazenar frutas
        std::map<std::pair<int, int>, std::unique_ptr<Fruit>> fruits;
        Bonus bonusFruit; // Fruta bônus
        
    public:
        Map(const sf::Vector2<float>& tileSize);
    
        bool loadFromFile(const std::string& filePath); // Carrega o mapa de um arquivo
        void reset(); // reseta o mapa depois de ter acontecido um gameover
        void draw(sf::RenderWindow& window);           // Desenha o mapa na janela
        const MapData& getMapData() const; // Retorna os dados do mapa

        void operator++(int);

        void updatePacman(const sf::Vector2<int> direction);
        void updateGhosts();
        int getFruitsRemaining() const;
        void setLevel(int currentLevel);
        void updateBonusFruit();

        int colision();
        void resetGhosts();

};

#endif