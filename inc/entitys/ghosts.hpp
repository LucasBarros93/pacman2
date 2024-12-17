#ifndef GHOST_HPP
#define GHOST_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <random>

#include "ghost_base.hpp"

typedef std::vector<std::vector <char>> MapData; // Tipo de dado para o mapa

// ============================
// Classe Blinky (fantasma vermelho)
// ============================
class Blinky : public Ghost{
    private:
        //Comportamento e animacao no modo NORMAL
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;
    
    public:
        Blinky(const std::string& texturePath, int fw, int fh, float fd, int df);
};

// ============================
// Classe Pinky (fantasma rosa)
// ============================
class Pinky : public Ghost{
    private:
        //Comportamento e animacao no modo NORMAL
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;
    
    public:
        Pinky(const std::string& texturePath, int fw, int fh, float fd, int df);
};

// ============================
// Classe Inky (fantasma azul)
// ============================
class Inky : public Ghost{
    private:
        //Comportamento e animacao no modo NORMAL
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;
 
    public:
        Inky(const std::string& texturePath, int fw, int fh, float fd, int df);
};

// ============================
// Classe Clyde (fantasma laranja)
// ============================
class Clyde : public Ghost{
    private:
        //Comportamento e animacao no modo NORMAL
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;

    public:
        Clyde(const std::string& texturePath, int fw, int fh, float fd, int df);
};


#endif // GHOST_HPP
