#include "score.hpp"

Score::Score(int val = 0) : value(val) {}


Score Score::operator+(int points) const {
    return Score(value + points);
}

Score& Score::operator+=(int points) {
    value += points;
    return *this;
}

int Score::getValue() const {
    return value;
}