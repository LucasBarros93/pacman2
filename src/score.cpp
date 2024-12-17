#include "score.hpp"

// Construtor da classe Score
// Inicializa o valor da pontuação (valor padrão = 0)
Score::Score(int val = 0) : value(val){}

// Sobrecarga do operador "+" para somar pontos
Score Score::operator+(int points) const{
    return Score(value + points); // Cria um novo objeto com o valor somado
}

// Sobrecarga do operador "+=" para adicionar pontos ao valor atual
Score& Score::operator+=(int points){
    value += points; // Adiciona os pontos ao valor atual
    return *this;    // Retorna o objeto modificado
}

// Método para acessar o valor da pontuação
int Score::getValue() const{
    return value; // Retorna o valor armazenado no objeto
}
