#include "game/GameState.hpp"

int GameState::getScore() const { return score; }

int GameState::getHighScore() const { return highScore; }

int GameState::getLives() const { return lives; }

int GameState::getRound() const { return round; }

void GameState::addScore(int points) {
    if (points <= 0)
        return;

    score += points;
}

void GameState::loseLife() {
    if (lives > 0)
        --lives;
}

void GameState::advanceLevel() { ++round; }

void GameState::reset() {
    score = 0;
    lives = 3;
    round = 1;
}

void GameState::setHighScore(int value) { highScore = value; }
