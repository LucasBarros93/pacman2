#include "game/Leaderboard.hpp"

#include <algorithm>
#include <fstream>
#include <utility>

Leaderboard::Leaderboard(std::string path) : filePath{std::move(path)} {
    load();
}

void Leaderboard::sortAndTrim() {
    std::stable_sort(entries.begin(), entries.end(),
                     [](const ScoreEntry &a, const ScoreEntry &b) {
                         return a.score > b.score;
                     });

    if (entries.size() > MAX_ENTRIES)
        entries.resize(MAX_ENTRIES);
}

void Leaderboard::load() {
    entries.clear();

    std::ifstream file(filePath);
    if (!file.is_open())
        return;

    ScoreEntry entry;
    while (file >> entry.name >> entry.score)
        entries.push_back(entry);

    sortAndTrim();
}

void Leaderboard::save() const {
    std::ofstream file(filePath);
    if (!file.is_open())
        return;

    for (const ScoreEntry &entry : entries)
        file << entry.name << ' ' << entry.score << '\n';
}

bool Leaderboard::qualifies(int score) const {
    if (score <= 0)
        return false;

    return entries.size() < MAX_ENTRIES || score > entries.back().score;
}

void Leaderboard::add(const std::string &name, int score) {
    entries.push_back({name, score});
    sortAndTrim();
    save();
}

const std::vector<ScoreEntry> &Leaderboard::getEntries() const {
    return entries;
}

int Leaderboard::getTopScore() const {
    return entries.empty() ? 0 : entries.front().score;
}
