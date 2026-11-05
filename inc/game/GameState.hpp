#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class GameState {
  private:
    int score = 0;
    int highScore = 0;
    int livesOnStart = 2;
    int lives = 2;
    int round = 1;

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
