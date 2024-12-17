#ifndef GHOST_BASE_HPP
#define GHOST_BASE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <random>

typedef std::vector<std::vector <char>> MapData; // Tipo de dado para o mapa

// ============================
// Classe base Ghost (Fantasma)
// ============================
// Classe que representa um fantasma generico, pq os outros vao se derivar dela  
// Ela define atributos e comportamentos comuns a todos os fantasmas
class Ghost{
    public: enum Mode { NORMAL, POWERLESS, DEAD, SPAWN, OUTGAME }; // Estados possiveis do fantasma
    protected:
        sf::Sprite sprite;          // Sprite do fantasma
        sf::Texture texture;        // Textura do fantasma
        sf::Vector2<int> dir;       // Direcao atual (-1, 0, ou 1 pra x e y)
        sf::Vector2<int> pos;       // Posicao atual no grid(x, y)
        int dificult;               // [0,99] 0 mto facil - 99 mto dificil
        int count;                  // contador de "turnos"
        sf::IntRect currentFrame;   // Frame atual pra animacao
        sf::Clock animationClock;   // Relogio para controle da animacao
        const int frameWidth;       // Largura do frame (ex: 16 pixels)
        const int frameHeight;      // Altura do frame (ex: 16 pixels)
        const int frameCount;       // Quantidade de frames por deirecao
        float frameDuration;        // Tempo entre frames
        int currentFrameIndex;      // Indice do frame atual
        Mode currentMode;           // Estados do fantasma
        MapData spawn(MapData mapData, char self); // Trata do spawn (nascimento) do fantasma
        MapData powerless(MapData mapData, char self); // Trata do modo "powerless" (fantasma enfraquecido q pode ser eliminado pelo pacman)
        virtual void updateAnimationNormal(); // Atualiza a animacao no estado NORMAL (pode ser sobrescrito)
        virtual MapData updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos); // Atualiza o comportamento no estado NORMAL (pode ser sobrescrito)
   
    public:
        Ghost(const std::string& texturePath, int fw, int fh, float fd, int df); // Construtor da classe base
        void updateAnimation(); // Atualiza a animacao com base no estado atual
        MapData updateBehavior(MapData mapData, char self, sf::Vector2<int>pacmanPos); // Atualiza o comportamento com base no estado atual
        void reset(const sf::Vector2<int>& position); // reseta dps que da gameover
        MapData kill(MapData mapData, char self); // Mata o fantasma e muda seu estado
        void setDirection(const sf::Vector2<int>& direction); // Define a direcao de movimento
        void setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize);// Define a posicao inicial
        void setMode(Mode mode); // Define o modo atual do fantasma                           
        void setDificult(int df); // Define a dificuldade do fantasma
        void setCount(int i); // Define o contador de "turnos"
        const sf::Sprite getSprite() const; // Retorna o sprite do fantasma
        const sf::Vector2<int>& getPosition() const; // Retorna a posicao atual do fantasna
        Mode getMode() const; // Retorna o modo atual do fantasma
};


#endif // GHOST_BASE_HPP
