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
    this->sprite.setPosition(this->pos.x * 1.005*tileSize.x, this->pos.y * 1.005*tileSize.y);
}

std::vector<std::vector <char>> Pacman::update(std::vector<std::vector <char>> mapData, const sf::Vector2<int> direction) {
    char to1 = '#', to2 = '#';
    if(direction.x != 0){
        to1 = mapData[this->pos.y+0][this->pos.x+direction.x];
        to2 = mapData[this->pos.y+1][this->pos.x+direction.x];
    } 
    if(direction.y != 0){
        to1 = mapData[this->pos.y+direction.y][this->pos.x+0];
        to2 = mapData[this->pos.y+direction.y][this->pos.x+1];
    } 

    if(to1 != '#' && to2 != '#')
        this->dir = direction;
    
    sf::Vector2<int> temp = this->pos + this->dir;
    if(mapData[temp.y][temp.x]     == '#' ||
       mapData[temp.y+1][temp.x]   == '#' ||
       mapData[temp.y][temp.x+1]   == '#' ||
       mapData[temp.y+1][temp.x+1] == '#')
        this->dir = {0,0};
    
    
    mapData[this->pos.y][this->pos.x] = ' ';
    mapData[this->pos.y+1][this->pos.x] = ' ';
    mapData[this->pos.y][this->pos.x+1] = ' ';
    mapData[this->pos.y+1][this->pos.x+1] = ' ';
    
    this->pos += this->dir;

    mapData[this->pos.y][this->pos.x]     = 'P';
    mapData[this->pos.y+1][this->pos.x]   = 'P';
    mapData[this->pos.y][this->pos.x+1]   = 'P';
    mapData[this->pos.y+1][this->pos.x+1] = 'P';

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
