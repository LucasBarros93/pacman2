#include "score_manager.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

ScoreManager::ScoreManager(const std::string& filePath) : filePath(filePath) {
    loadScores();
}

void ScoreManager::loadScores() {
    scores.clear();
    std::ifstream file(this->filePath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pontuações: " << this->filePath << std::endl;
        return;
    }

    std::string name;
    int score;
    while (file >> name >> score) {
        scores.emplace_back(name, score);
    }

    file.close();
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;  // Ordena em ordem decrescente de pontuação
    });
}

void ScoreManager::saveScore(const std::string& playerName, int score) {
    scores.emplace_back(playerName, score);

    // Ordena as pontuações em ordem decrescente
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Mantém apenas os 5 melhores scores (ajuste conforme necessário)
    if (scores.size() > 5) {
        scores.resize(5);
    }

    std::ofstream file(this->filePath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pontuações para escrita!" << std::endl;
        return;
    }

    for (const auto& [name, score] : scores) {
        file << name << " " << score << "\n";
    }
    file.close();
}

std::vector<std::pair<std::string, int>> ScoreManager::getScores() const {
    return scores; // Retorna diretamente a lista de scores
}