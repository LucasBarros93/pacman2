#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <cstddef>
#include <string>
#include <vector>

/// @brief One line of the leaderboard.
struct ScoreEntry {
    std::string name; ///< The player's name (no spaces).
    int score = 0;    ///< Final score of the game.
};

/// @brief The best scores, kept in a text file.
///
/// The file has one entry per line, as `NAME SCORE` (for example
/// `ABC 12340`). A missing file just means an empty leaderboard. The entries
/// are always sorted from the highest score to the lowest, and only the best
/// MAX_ENTRIES are kept. With equal scores, the older entry stays above.
class Leaderboard {
  private:
    std::string filePath; ///< File the scores are read from and saved to.
    std::vector<ScoreEntry> entries; ///< Sorted, highest score first.

    /// @brief Reads the file, replacing the current entries.
    void load();

    /// @brief Writes the entries to the file. If the file cannot be written,
    ///        the scores are only kept until the game closes.
    void save() const;

    /// @brief Sorts the entries by score and drops those that do not fit.
    void sortAndTrim();

  public:
    static constexpr std::size_t MAX_ENTRIES = 5; ///< How many scores are kept.

    /// @param path Path of the scores file. It is loaded right away.
    explicit Leaderboard(std::string path);

    /// @brief Tells whether a score earns a place on the leaderboard.
    /// @param score The final score of a game.
    /// @return true if it is above zero and either there is still room or it
    ///         beats the last place (a tie does not).
    [[nodiscard]] bool qualifies(int score) const;

    /// @brief Adds a score and saves the file.
    /// @param name The player's name (no spaces).
    /// @param score The final score.
    void add(const std::string &name, int score);

    /// @brief The entries, from the highest score to the lowest.
    [[nodiscard]] const std::vector<ScoreEntry> &getEntries() const;

    /// @brief The best score on the leaderboard, or 0 if it is empty.
    [[nodiscard]] int getTopScore() const;
};

#endif
