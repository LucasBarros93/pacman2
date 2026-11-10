#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class GameState {
  private:
    static constexpr int EXTRA_LIFE_SCORE = 10000;

    int score = 0;
    int highScore = 0;
    int livesOnStart = 2;
    int lives = 2;
    int round = 1;
    bool extraLifeGiven = false;

  public:
    int getScore() const;
    int getHighScore() const;
    int getLives() const;
    int getRound() const;

    void addScore(int points);
    void loseLife();
    void advanceLevel();

    void reset();
    void setHighScore(int value);
};

#endif
