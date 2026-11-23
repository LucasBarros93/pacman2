#include "game/GameState.hpp"

int GameState::getScore() const { return score; }

int GameState::getHighScore() const { return highScore; }

int GameState::getLives() const { return lives; }

int GameState::getRound() const { return round; }

bool GameState::isGameOver() const { return lives < 0; }

bool GameState::addScore(int points) {
    if (points <= 0)
        return false;

    score += points;

    if (!extraLifeGiven && score >= EXTRA_LIFE_SCORE) {
        extraLifeGiven = true;
        ++lives;
        return true;
    }

    return false;
}

void GameState::loseLife() {
    if (lives >= 0)
        --lives;
}

void GameState::advanceLevel() { ++round; }

void GameState::reset() {
    score = 0;
    lives = LIVES_ON_START;
    round = 1;
    extraLifeGiven = false;
}

void GameState::setHighScore(int value) { highScore = value; }
