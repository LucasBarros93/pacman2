#ifndef SCORE_HPP
#define SCORE_HPP

#include <iostream>

// ============================
// Classe Score
// ============================
// A classe Score eh responsavel por gerenciar o sistema de pontuação do jogo, aquela que fica sendo mostrada "ao vivo" enquanto o jogador joga
class Score{
    private:
        int value; //Armazena o valor atual da pontuaacao

    public:
        Score(int val); // Construtor inicia a pontuacao com um valor inicial

        // ============================
        // Sobrecarga do operador +
        // ============================
        // Permite adicionar pontos ao objeto Score sem precisar modificar o valor original
        // Retorna um novo objeto Score com a soma do valor atual e dos pontos fornecidos
        Score operator+(int points) const;

        // ============================
        // Sobrecarga do operador +=
        // ============================
        // Permite adicionar pontos ao objeto Score diretamente, alterando seu valor
        // Retorna uma referencia ao proprio objeto pra permitir chamadas encadeadas
        Score& operator+=(int points);

        int getValue() const; // Da o valor atual da pontuacao
};

#endif
