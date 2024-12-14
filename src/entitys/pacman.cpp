#include "entitys/pacman.hpp"


Pacman::Pacman(const std::string& texturePath, float speed)
    : dir(0, 0), speed(speed) {

    if (!this->texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Erro ao carregar a textura do Pacman!");
    }
    this->sprite.setTexture(this->texture);
    this->sprite.setScale(0.05f, 0.05f); // Ajuste de escala, se necessário
}

void Pacman::setDirection(const sf::Vector2<int>& direction) {
    this->dir = direction;
}

void Pacman::setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize) {
    this->pos = position;
    this->sprite.setPosition(this->pos.x * tileSize.x, this->pos.y * tileSize.y);
}

std::vector<std::vector <char>> Pacman::update(std::vector<std::vector <char>> mapData, const sf::Vector2<int> direction) {
    char to = mapData[this->pos.x+direction.x][this->pos.y+direction.y];
    return mapData;

}

const sf::Sprite Pacman::getSprite() const{
    return this->sprite;
}

const sf::Vector2<int>& Pacman::getPosition() const {
    return this->pos;
}

const sf::Vector2<int>& Pacman::getDirection() const {
    return this->dir;
}
