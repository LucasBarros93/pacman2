#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

/// @brief The numbers of the current game: score, lives and round.
///
/// Lives work like in the original game: they are the spare lives (the ones
/// drawn in the HUD). The player starts with 2, and the game is over when
/// one is lost with none left, which makes the count go to -1.
///
/// The high score is frozen while a game is played: it only changes at the
/// start of a game, with setHighScore().
class GameState {
  private:
    static constexpr int EXTRA_LIFE_SCORE =
        10000; ///< Score that gives the extra life (once per game).
    static constexpr int LIVES_ON_START =
        2; ///< Spare lives at the start of a game.

    int score = 0;
    int highScore = 0;
    int lives = LIVES_ON_START;
    int round = 1;
    bool extraLifeGiven = false;

  public:
    [[nodiscard]] int getScore() const;

    /// @brief The high score shown in the HUD. It does not change during a
    ///        game (see setHighScore()).
    [[nodiscard]] int getHighScore() const;

    /// @brief Spare lives, the ones drawn in the HUD. It is -1 when the game
    ///        is over.
    [[nodiscard]] int getLives() const;

    /// @brief The current round, starting at 1.
    [[nodiscard]] int getRound() const;

    /// @brief Tells whether the player lost the last life.
    [[nodiscard]] bool isGameOver() const;

    /// @brief Adds points to the score.
    ///
    /// Gives the extra life when the score reaches 10000 (only once per game).
    /// Zero or negative values are ignored.
    /// @param points How many points to add.
    void addScore(int points);

    /// @brief Takes one life away. With no spare lives left the count goes to
    ///        -1 (game over) and stays there.
    void loseLife();

    /// @brief Goes to the next round.
    void advanceLevel();

    /// @brief Starts a new game: score 0, spare lives back to 2, round 1. The
    ///        high score is kept.
    void reset();

    /// @brief Sets the high score shown during the game. It is called at the
    ///        start of each game with the top of the leaderboard.
    /// @param value The new high score.
    void setHighScore(int value);
};

#endif
