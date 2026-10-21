#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

class GameState {
  private:
    int score = 0;
    int highScore = 0;
    int lives = 3;
    int level = 1;

  public:
    int getScore() const;
    int getHighScore() const;
    int getLives() const;
    int getLevel() const;

    void addScore(int points);
    void loseLife();
    void advanceLevel();
};

#endif
