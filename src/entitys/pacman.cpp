#include "entitys/pacman.hpp"

Pacman::Pacman(const std::string& texturePath, const sf::Vector2f& startPos, float speed)
    : pos(startPos), dir(0, 0), speed(speed) {
    if (!texture.loadFromFile(texturePath)) {
        throw std::runtime_error("Erro ao carregar a textura do Pacman!");
    }
    sprite.setTexture(texture);
    sprite.setPosition(pos);
    sprite.setScale(0.8f, 0.8f); // Ajuste de escala, se necessário
}

void Pacman::setDirection(const sf::Vector2f& direction) {
    dir = direction;
}

void Pacman::update(float deltaTime) {
    pos += dir * speed * deltaTime; // Atualiza a posição com base na direção e na velocidade
    sprite.setPosition(pos);
}

void Pacman::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

const sf::Vector2f& Pacman::getPosition() const {
    return pos;
}

const sf::Vector2f& Pacman::getDirection() const {
    return dir;
}
