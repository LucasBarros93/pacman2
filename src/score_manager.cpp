#include "score_manager.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

// Construtor da classe ScoreManager
// Inicializa o caminho do arquivo e carrega as pontuações do arquivo
ScoreManager::ScoreManager(const std::string& filePath) : filePath(filePath) {
    loadScores(); // Carrega as pontuações ao criar o gerenciador
}

// Método para carregar as pontuações do arquivo
void ScoreManager::loadScores() {
    scores.clear(); // Limpa a lista de pontuações
    
    std::ifstream file(this->filePath); // Abre o arquivo para leitura

    // Verifica se o arquivo foi aberto corretamente
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pontuações: " << this->filePath << std::endl;
        return;
    }

    std::string name; // Nome do jogador
    int score; // Pontuação do jogador

    // Lê o arquivo
    while (file >> name >> score)
        scores.emplace_back(name, score); // Adiciona o par (nome, pontuação) a lista

    file.close();// Fecha o arquivo

    // Ordena as pontuações em ordem decrescente
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
}

// Método para salvar uma nova pontuação
void ScoreManager::saveScore(const std::string& playerName, int score) {
    scores.emplace_back(playerName, score); // Adiciona o novo score à lista

    // Ordena as pontuações em ordem decrescente
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Mantém apenas os 5 melhores scores
    if (scores.size() > 5)
        scores.resize(5);

    // Abre o arquivo para escrita (sobrescrevendo o conteúdo anterior)
    std::ofstream file(this->filePath);

    // Verifica se o arquivo foi aberto corretamente
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pontuações para escrita!" << std::endl;
        return;
    }

    // Escreve as pontuações no arquivo
    for (const auto& [name, score] : scores)
        file << name << " " << score << "\n";
    
    file.close(); // Fecha o arquivo
}

// Método para obter as pontuações atuais
std::vector<std::pair<std::string, int>> ScoreManager::getScores() const {
    return scores; // Retorna a lista de pontuações
}