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
Bonus::Bonus(int fw, int fh) 
    : frameWidth(fw), frameHeight(fh), active(false) {
    this->frame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight);
    this->points = 0; // Inicializa pontos
}

void Bonus::spawn(const std::vector<std::vector<char>>& mapData) {
    // Verifica se a fruta bônus já está ativa
    if (this->active) return;

    // Encontra posições livres no mapa
    std::vector<sf::Vector2<int>> emptyPositions;
    for (size_t y = 0; y < mapData.size(); ++y) {
        for (size_t x = 0; x < mapData[y].size(); ++x) {
            if (mapData[y][x] == ' ') {
                emptyPositions.push_back({static_cast<int>(x), static_cast<int>(y)});
            }
        }
    }

    if (!emptyPositions.empty()) {
        // Gera uma posição aleatória
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, emptyPositions.size() - 1);
        this->pos = emptyPositions[dist(gen)];

        // Aleatoriza pontos e textura
        randomize();

        // Ativa a fruta bônus
        this->active = true;
        this->visibilityTimer.restart(); // Reinicia o tempo de visibilidade
    }
}

void Bonus::update(const std::vector<std::vector<char>>& mapData) {
    if (active) {
        // Se a fruta estiver visível por mais de 10 segundos, desativa
        if (visibilityTimer.getElapsedTime().asSeconds() > 10.0f) {
            active = false;
            respawnTimer.restart(); // Inicia o temporizador de respawn
        }
    } else {
        // Se a fruta estiver inativa por 15 segundos, reaparece
        if (respawnTimer.getElapsedTime().asSeconds() > 15.0f) {
            spawn(mapData); // Passa o mapa como argumento
        }
    }
}



void Bonus::randomize() {
    const int pointsOptions[8] = {100, 300, 500, 700, 1000, 2000, 3000, 5000};
    int textureIndex = getRandomNumber(0, 7);

    std::string texturePath = "./assets/images/spritesheet.png";
    if (!texture.loadFromFile(texturePath)) {
        std::cerr << "Erro ao carregar textura: " << texturePath << std::endl;
    }

    int offsetX = (textureIndex * 10) + 32;
    this->frame.left = offsetX;
    this->frame.top = 64;

    this->sprite.setTexture(texture);
    this->sprite.setTextureRect(this->frame);
    this->points = pointsOptions[textureIndex];
}

int Bonus::getPoints() const {
    return this->points;
}

void Bonus::draw(sf::RenderWindow& window, const sf::Vector2f& offset) const {
    if (this->active) {
        sf::Sprite spriteCopy = this->sprite;
        spriteCopy.setPosition((this->pos.x * 10) + offset.x, (this->pos.y * 10) + offset.y);
        window.draw(spriteCopy);
    }
}

void Bonus::reset() {
    this->active = false;            // Desativa a fruta bônus
    this->points = 0;                // Reseta os pontos
    this->pos = {0, 0};              // Reseta a posição
    this->respawnTimer.restart();    // Inicia o temporizador de respawn
}



bool Bonus::isActive() const {
    return this->active;
}

sf::Vector2<int> Bonus::getPosition() const {
    return this->pos;
}


