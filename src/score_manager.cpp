#include "score_manager.hpp"

ScoreManager::ScoreManager(const std::string& filePath) : filePath(filePath) {
    loadScores();
}

void ScoreManager::loadScores() {
    scores.clear();
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de pontuações: " << filePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        ScoreEntry entry;
        stream >> entry.name >> entry.score;
        scores.push_back(entry);
    }

    file.close();
    std::sort(scores.begin(), scores.end());
}

void ScoreManager::saveScores() {
    std::ofstream file(filePath, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Erro ao salvar o arquivo de pontuações: " << filePath << std::endl;
        return;
    }

    for (size_t i = 0; i < std::min(scores.size(), static_cast<size_t>(5)); ++i) {
        file << scores[i].name << " " << scores[i].score << std::endl;
    }

    file.close();
}

void ScoreManager::addScore(const std::string& name, int score) {
    scores.push_back({name, score});
    std::sort(scores.begin(), scores.end());
    saveScores();
}

const std::vector<ScoreEntry>& ScoreManager::getScores() const {
    return scores;
}
