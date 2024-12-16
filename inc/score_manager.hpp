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

    ScoreEntry(const std::string& name, int score) : name(name), score(score) {}
};


class ScoreManager {
public:
    ScoreManager(const std::string& filePath);
    void saveScore(const std::string& playerName, int score);
    std::vector<std::pair<std::string, int>> getScores() const;


private:
    std::string filePath;
    std::vector<std::pair<std::string, int>> scores;
    void loadScores();
};

#endif