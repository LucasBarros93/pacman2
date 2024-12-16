#ifndef SCORE_HPP
#define SCORE_HPP

#include <iostream>

class Score {
private:
    int value;

public:
    Score(int val = 0) : value(val) {}

    // Sobrecarga do operador +
    Score operator+(int points) const {
        return Score(value + points);
    }

    // Sobrecarga de operador de atribuição +=
    Score& operator+=(int points) {
        value += points;
        return *this;
    }

    int getValue() const {
        return value;
    }
};

#endif
