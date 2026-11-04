#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <cstddef>
#include <string>
#include <vector>

struct ScoreEntry {
    std::string name;
    int score = 0;
};

class Leaderboard {
  private:
    std::string filePath;
    std::vector<ScoreEntry> entries;

    void sortAndTrim();

  public:
    static constexpr std::size_t MAX_ENTRIES = 5;

    explicit Leaderboard(std::string path);

    void load();
    void save() const;

    bool qualifies(int score) const;
    void add(const std::string &name, int score);

    const std::vector<ScoreEntry> &getEntries() const;
    int getTopScore() const;
};

#endif
