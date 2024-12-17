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
        sf::Vector2<float> pos;
        int points;

        sf::IntRect frame;
        const int frameWidth; 
        const int frameHeight;

        void randomize(); // Aleatoriza a textura da fruta

    public:
        Bonus(int fw, int fh);
        void spawn();
        int getPoints() const override;
        void draw(sf::RenderWindow& window, const sf::Vector2f& offset) const override;
};

#endif // FRUITS_HPP