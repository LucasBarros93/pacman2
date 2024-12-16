#ifndef GHOST_HPP
#define GHOST_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

typedef std::vector<std::vector <char>> MapData;

class Ghost {
    public: enum Mode { NORMAL, POWERLESS, DEAD, SPAWN };
    protected:
        sf::Sprite sprite;          // Sprite do fantasma
        sf::Texture texture;        // Textura do fantasma
        sf::Vector2<int> pos;       // Posição atual (x, y)
        sf::Vector2<int> dir;       // Direção atual (-1, 0, ou 1 para x e y)

        sf::IntRect currentFrame;   // Frame atual para animação
        sf::Clock animationClock;   // Relógio para controle da animação

        const int frameWidth;       // Largura do frame (ex: 16 pixels)
        const int frameHeight;      // Altura do frame (ex: 16 pixels)
        const int frameCount;       // Quantidade de frames por direção
        float frameDuration;        // Tempo entre frames
        int currentFrameIndex;      // Índice do frame atual

        Mode currentMode; // Estados do fantasma

    public:
        Ghost(const std::string& texturePath, int fw, int fh, float fd);

        MapData spawn(MapData mapData, char self);

        // Atualiza a animação e comportamento com base no estado
        virtual void updateAnimationNormal();
        virtual MapData updateBehaviorNormal(MapData mapData);

        virtual void updateAnimation();
        virtual MapData updateBehavior(MapData mapData, char self);

        void setDirection(const sf::Vector2<int>& direction);  // Define a direção
        void setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize);    // Define a posição
        void setMode(Mode mode);                               // Define o modo atual

        const sf::Sprite getSprite() const;                    // Retorna o sprite
        const sf::Vector2<int>& getPosition() const;           // Retorna a posição
        Mode getMode() const;                                  // Retorna o modo atual
};

class Blinky : public Ghost {
    public:
        Blinky(const std::string& texturePath, int fw, int fh, float fd);

        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData) override;
        void updateAnimationNormal() override;
};

class Pinky : public Ghost {
    public:
        Pinky(const std::string& texturePath, int fw, int fh, float fd);

        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData) override;
        void updateAnimationNormal() override;
};

class Inky : public Ghost {
    public:
        Inky(const std::string& texturePath, int fw, int fh, float fd);

        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData) override;
        void updateAnimationNormal() override;
};

class Clyde : public Ghost {
    public:
        Clyde(const std::string& texturePath, int fw, int fh, float fd);

        // Comportamento específico de Blinky
        MapData updateBehaviorNormal(MapData mapData) override;
        void updateAnimationNormal() override;
};


#endif // GHOST_HPP
