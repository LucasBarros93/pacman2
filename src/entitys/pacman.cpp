#include "entitys/pacman.hpp"

Pacman::Pacman(){
    this->dir= {0,0};
    this->speed = 0;
}

Pacman::Pacman(const std::string& texturePath, const sf::Vector2f& startPos, float speed)
    : pos(startPos), dir(0, 0), speed(speed) {

    if (!this->texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Erro ao carregar a textura do Pacman!");
    }
    this->sprite.setTexture(this->texture);
    this->sprite.setPosition(this->pos);
    this->sprite.setScale(0.05f, 0.05f); // Ajuste de escala, se necessário
}

void Pacman::setDirection(const sf::Vector2f& direction) {
    this->dir = direction;
}

void Pacman::setPosition(const sf::Vector2f& position) {
    this->pos = position;
    this->sprite.setPosition(this->pos);
}

void Pacman::update(float deltaTime) {
    this->pos += this->dir * speed * deltaTime; // Atualiza a posição com base na direção e na velocidade
    this->sprite.setPosition(this->pos);
}

const sf::Sprite Pacman::getSprite() const{
    return this->sprite;
}

const sf::Vector2f& Pacman::getPosition() const {
    return this->pos;
}

const sf::Vector2f& Pacman::getDirection() const {
    return this->dir;
}
