#ifndef SCORE_MANAGER_HPP
#define SCORE_MANAGER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>


// Esse arquivo eh diferente do "score.hpp" pq ele trata do score armazenado no quadro de lideres, enquanto o outro lida com o score atualizado "ao vivo"

// Essa struct eh uma entrada individual no quadro de lideres
struct ScoreEntry{
    std::string name; // Nome do jogador
    int score; // Pontuacao do jogador
    ScoreEntry(const std::string& name, int score) : name(name), score(score) {} // Construtor q so inicia o nome e o score
};

// =====================================
// Classe ScoreManager
// =====================================
// Lida com o sistema de quadro de lideres:
// - Carrega pontos de um arquivo
// - Salva novas pontuacoes
class ScoreManager{
    private:
        std::string filePath; // Caminho de onde ta o .txt com o top5
        std::vector<std::pair<std::string, int>> scores; // Vetor pra armazenar as pontuacoes e os nomes
        void loadScores(); // Carrega as pontuacoes e nomes do arquivo e depois escreve elas no vetor "scores"
    
    public:
        ScoreManager(const std::string& filePath);
        void saveScore(const std::string& playerName, int score); //Salva uma nova pontuacao no quadro de lideres
        std::vector<std::pair<std::string, int>> getScores() const; // Retorna o vetor de pontuacoes e nomes 
};

#endif