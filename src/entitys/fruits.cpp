#include "entitys/fruits.hpp"

// -----------------------------------
// Função auxiliar para números aleatórios
int getRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(mt);
}

// ===================================
// Implementação da classe Dot
Dot::Dot(sf::Vector2<int> position) : pos(position), points(10) {}

int Dot::getPoints() const {
    return this->points;
}

void Dot::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    sf::CircleShape dot(2.0f);
    dot.setFillColor(sf::Color(184, 197, 201));
    dot.setPosition(((this->pos.x * 10) + 10) + offset.x, ((this->pos.y * 10 )+10) + offset.y);
    window.draw(dot);
}

// ===================================
// Implementação da classe Energizer
Energizer::Energizer(sf::Vector2<int> position) : pos(position), points(50) {}

int Energizer::getPoints() const {
    return this->points;
}

void Energizer::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    sf::CircleShape energizer(4.0f);
    energizer.setFillColor(sf::Color(184, 197, 201));
    energizer.setPosition(((this->pos.x * 10) + 10) + offset.x, ((this->pos.y * 10 )+10) + offset.y);
    window.draw(energizer);
}

// ===================================
// Implementação da classe Bonus
Bonus::Bonus(int fw, int fh) : frameWidth(fw), frameHeight(fh) {
    this->frame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight);
    spawn();
}

void Bonus::spawn() {
    this->pos = {static_cast<float>(getRandomNumber(0, 20) * 10), 
                static_cast<float>(getRandomNumber(0, 20) * 10)};
    randomize();
}

void Bonus::randomize() {
    // Aleatoriza a textura da fruta (pode ter várias opções)
    const int points[8] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};
    int textureIndex = getRandomNumber(0, 7);
    std::string texturePath = "./assets/images/spritesheet.png";
    if (!texture.loadFromFile(texturePath))
        std::cerr << "Erro ao carregar textura: " << texturePath << std::endl;
    
    int offsetX = (textureIndex * 10) + 32;

    this->frame.left = offsetX;
    this->frame.top = 64;

    this->sprite.setTextureRect(frame);
    this->sprite.setPosition(this->pos);

    this->points = points[textureIndex];
}

int Bonus::getPoints() const {
    return this->points;
}

void Bonus::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    // Cria uma cópia do sprite do Bonus
    sf::Sprite spriteCopy = this->sprite;
    // Aplica o offset à posição
    spriteCopy.setPosition(spriteCopy.getPosition() + offset);
    window.draw(spriteCopy);
}

