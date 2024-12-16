#ifndef SCORE_HPP
#define SCORE_HPP

#include <iostream>

class Score {
    private:
        int value;

    public:
        Score(int val);

        // Sobrecarga do operador +
        Score operator+(int points) const;

        // Sobrecarga de operador de atribuição +=
        Score& operator+=(int points);

        int getValue() const;
};

#endif
