#ifndef GHOST_HPP
#define GHOST_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <random>

typedef std::vector<std::vector <char>> MapData;

class Ghost {
    public: enum Mode { NORMAL, POWERLESS, DEAD, SPAWN, OUTGAME };
    protected:
        sf::Sprite sprite;          // Sprite do fantasma
        sf::Texture texture;        // Textura do fantasma
        sf::Vector2<int> dir;       // Direção atual (-1, 0, ou 1 para x e y)
        sf::Vector2<int> pos;       // Posição atual (x, y)

        int dificult;               // [0,99] 0 mto facil - 99 mto dificil
        int count;                  // contador de "turnos"

        sf::IntRect currentFrame;   // Frame atual para animação
        sf::Clock animationClock;   // Relógio para controle da animação

        const int frameWidth;       // Largura do frame (ex: 16 pixels)
        const int frameHeight;      // Altura do frame (ex: 16 pixels)
        const int frameCount;       // Quantidade de frames por direção
        float frameDuration;        // Tempo entre frames
        int currentFrameIndex;      // Índice do frame atual

        Mode currentMode; // Estados do fantasma

        MapData spawn(MapData mapData, char self);
        MapData powerless(MapData mapData, char self);

        virtual void updateAnimationNormal();
        virtual MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos);
   
    public:
        Ghost(const std::string& texturePath, int fw, int fh, float fd, int df);

        // Atualiza a animação e comportamento com base no estado
        void updateAnimation();
        MapData updateBehavior(MapData mapData, char self, sf::Vector2<int>pacmanPos);
        void reset(const sf::Vector2<int>& position, Mode mode); // reseta dps que da gameover

        MapData kill(MapData mapData, char self);

        void setDirection(const sf::Vector2<int>& direction);  // Define a direção
        void setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize);    // Define a posição
        void setMode(Mode mode);                               // Define o modo atual
        void setDificult(int df);
        void setCount(int i);

        const sf::Sprite getSprite() const;                    // Retorna o sprite
        const sf::Vector2<int>& getPosition() const;           // Retorna a posição
        Mode getMode() const;                                  // Retorna o modo atual
};

class Blinky : public Ghost {
    private:
        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;
    
    public:
        Blinky(const std::string& texturePath, int fw, int fh, float fd, int df);
};

class Pinky : public Ghost {
    private:
        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;
    
    public:
        Pinky(const std::string& texturePath, int fw, int fh, float fd, int df);
};

class Inky : public Ghost {
    private:
        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;
 
    public:
        Inky(const std::string& texturePath, int fw, int fh, float fd, int df);
};

class Clyde : public Ghost {
    private:
        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) override;
        void updateAnimationNormal() override;

    public:
        Clyde(const std::string& texturePath, int fw, int fh, float fd, int df);
};


#endif // GHOST_HPP
