#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "entitys/pacman.hpp"
#include "entitys/ghosts.hpp"
#include "entitys/fruits.hpp"

typedef std::vector<std::vector <char>> MapData; // Define um tipo de dado para representar o mapa (matriz de caracteres que esta definida num .txt na pasta "assets")

// ============================
// Classe Map
// ============================
// Classe responsavel por gerenciar o mapa, as entidades e a logica do jogo
class Map{
    private:
        MapData mapData;  // Matriz q representa o mapa
        int count; // Contador para gerenciar atualizacoes ou eventos no jogo
        int level; // Pega o level que o jogador esta atualemnte pra poder modificar a dificuldade
        sf::Vector2<float> tileSize; // Tamanho de cada celula (tile) do mapa
        sf::Vector2f mapOffset; // Offset para centralizar o mapa na tela, se deixar natural ele vai seguir o map.txt e ficar encostado no canto direito
        sf::RectangleShape wall; // Imagem das paredes
        sf::CircleShape dot; // Imagem dos pontos
        // Entidades/objetos do jogo
        Pacman pac;
        Blinky blinky;
        Pinky pinky;
        Inky inky;
        Clyde clyde;
        // Lidando com as frutas abaixo
        std::map<std::pair<int, int>, std::unique_ptr<Fruit>> fruits; // Armazena os dots (ptos pequenos) e energizers (ptos grandes) e suas posicoes
        Bonus bonusFruit; // Fruta bonus (fruta de imagem igual as frutas de verdade, cereja etc)
        
    public:
        Map(const sf::Vector2<float>& tileSize); // O construtor inicia o mapa com o tamanho dos tiles    
        bool loadFromFile(const std::string& filePath); // Carrega o mapa de um arquivo
        void reset(); // reseta o mapa depois de ter acontecido um gameover
        void draw(sf::RenderWindow& window); // Desenha o mapa e as outras coisas na janela
        const MapData& getMapData() const; // Retorna os dados do mapa, assim tem como as outras classes acessarem
        void operator++(int); // Operador pra incrementar, usado em casos especificos como spawn de fantasmas
        void updatePacman(const sf::Vector2<int> direction); // Atualiza a posicao do pacman com base na direcao fornecida
        void updateGhosts(); // atualiza comportamento de movimento e animacao dos fantasmas
        int getFruitsRemaining() const; // Retorna o numero de frutas restantes no mapa, na main usamos isso pra verificar se ja acabou uma fase ou n
        void setLevel(int currentLevel); // Define o nivel atual do jogador, serve pra mudar a dificuldade do jogo (muda a 'inteligencia' dos fantasmas)
        void updateBonusFruit(); // Atualiza o estado da frutinha bonus (spawn, temporizador, etc.)
        int colision(); // Esse eh importante demais. Ele identifica colisoes entre o pacman, os fantasmas e as frutas, retorna os pontos coletados
};

#endif