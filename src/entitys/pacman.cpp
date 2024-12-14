#include "entitys/pacman.hpp"


Pacman::Pacman(const std::string& texturePath, int fw, int fh, float fd, float speed)
    : frameWidth(fw), frameHeight(fh), frameCount(2), frameDuration(fd),
          currentFrameIndex(0), dir(0, 0), speed(speed) {

    if (!texture.loadFromFile(texturePath)) {
            throw std::runtime_error("Erro ao carregar spritesheet!");
        }
    sprite.setTexture(texture);

    // Configura o primeiro frame (direita)
    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, frameHeight);
    sprite.setTextureRect(this->currentFrame);

    this->sprite.setScale(1.f, 1.f); // Ajuste de escala, se necessário
}

void Pacman::setDirection(const sf::Vector2<int>& direction) {
    this->dir = direction;
    int auxDir;

    if (this->dir.x == 1)
        auxDir = 0;
    else if (this->dir.x == -1)
        auxDir = 1;
    else if (this->dir.y == 1)
        auxDir = 3;
    else if (this->dir.y == -1)
        auxDir = 2;

    this->currentFrame.top = auxDir * frameHeight; // Ajusta a linha na spritesheet
    this->currentFrameIndex = 0;                     // Reinicia o frame da animação
    this->currentFrame.left = 0;
    this->sprite.setTextureRect(this->currentFrame);

}

void Pacman::updateAnimation() {
    if (this->dir == sf::Vector2<int>(0, 0)) { 
        this->currentFrame.left = 2 * this->frameWidth; // Terceiro quadro (2 * largura do frame)
        this->currentFrame.top = 0;              // Primeira linha
    }
    else if (this->animationClock.getElapsedTime().asSeconds() > this->frameDuration) {
        this->animationClock.restart();

        // Avança para o próximo frame
        this->currentFrameIndex = (this->currentFrameIndex + 1) % this->frameCount;
        this->currentFrame.left = this->currentFrameIndex * this->frameWidth;

    }
    this->sprite.setTextureRect(this->currentFrame);
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
        this->setDirection(direction);
    
    sf::Vector2<int> temp = this->pos + this->dir;
    if(mapData[temp.y][temp.x]     == '#' ||
       mapData[temp.y+1][temp.x]   == '#' ||
       mapData[temp.y][temp.x+1]   == '#' ||
       mapData[temp.y+1][temp.x+1] == '#')
        this->setDirection({0,0});
    
    
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
