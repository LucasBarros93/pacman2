#ifndef FRUITS_HPP
#define FRUITS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

// Classe base abstrata polimórfica
class Fruit {
    public:
        virtual ~Fruit() = default;

        virtual int getPoints() const = 0; // Pontos da fruta
        virtual void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const = 0; // Desenha a fruta
};

// Classe Dot
class Dot : public Fruit {
    private:
        sf::Vector2<int> pos; // Posição da fruta no grid
        int points;

    public:
        Dot() : pos(0, 0), points(10) {};
        Dot(sf::Vector2<int> position);
        int getPoints() const override;
        void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const override;
};

// Classe Energizer
class Energizer : public Fruit {
    private:
        sf::Vector2<int> pos;
        int points;

    public:
        Energizer() : pos(0, 0), points(50) {};
        Energizer(sf::Vector2<int> position);
        int getPoints() const override;
        void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const override;
};

// Classe Bonus
class Bonus : public Fruit {
    private:
        sf::Sprite sprite;
        sf::Texture texture;
        sf::Vector2<int> pos; // Posição no grid
        int points;

        sf::IntRect frame;
        const int frameWidth;
        const int frameHeight;

        bool active;          // Estado da fruta (ativa ou não)
        sf::Clock visibilityTimer; // Temporizador para o tempo visível
        sf::Clock respawnTimer;    // Temporizador para o tempo inativo

        void randomize(); // Aleatoriza a textura e pontos da fruta

    public:
        Bonus(int fw, int fh);
        void spawn(const std::vector<std::vector<char>>& mapData); // Adapta para respeitar o mapa
        void update(const std::vector<std::vector<char>>& mapData);  // Atualiza o estado (ativa ou inativa)
        int getPoints() const override;
        void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const override;
        void reset();
        bool isActive() const; // Verifica se está ativa
        sf::Vector2<int> getPosition() const; // Posição no grid
};


#endif // FRUITS_HPP