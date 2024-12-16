#ifndef SCORE_MANAGER_HPP
#define SCORE_MANAGER_HPP

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iostream>

struct ScoreEntry {
    std::string name;
    int score;

    bool operator<(const ScoreEntry& other) const {
        return score > other.score; // Ordenar em ordem decrescente
    }
};

class ScoreManager {
private:
    std::vector<ScoreEntry> scores;
    const std::string filePath;

    void loadScores();
    void saveScores();

public:
    ScoreManager(const std::string& filePath);

    void addScore(const std::string& name, int score);
    const std::vector<ScoreEntry>& getScores() const;
};

#endif
