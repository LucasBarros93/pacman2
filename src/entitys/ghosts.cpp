#include "entitys/ghosts.hpp"

Ghost::Ghost(const std::string& texturePath, int fw, int fh, float fd)
    : dir(0, 0), frameWidth(fw), frameHeight(fh), frameCount(2), frameDuration(fd),
      currentFrameIndex(0),  currentMode(NORMAL) {

    if (!this->texture.loadFromFile(texturePath))
        throw std::runtime_error("Erro ao carregar spritesheet dos fantasmas!");
    
    this->sprite.setTexture(this->texture);

    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight);
    this->sprite.setTextureRect(this->currentFrame);

    this->sprite.setScale(1.f, 1.f); // Ajuste de escala, se necessário
}

void Ghost::updateAnimation() {
    if (this->animationClock.getElapsedTime().asSeconds() > this->frameDuration) {
        this->animationClock.restart();
        
        this->currentFrameIndex = (this->currentFrameIndex + 1) % this->frameCount;

        int offsetY = 0;
        int offsetX = 0;

        switch (this->currentMode) {
            case NORMAL:
                // Frames normais para fantasmas
                this->updateAnimationNormal();
                return;
                break;

            case POWERLESS:
                // Frames "powerless" (linha 4, colunas 0 e 1)
                offsetY = 80;  // Linha 4 (64px * 2)
                offsetX = 128 + (this->currentFrameIndex * this->frameWidth);
                break;

            case DEAD:
                // Frames "dead" (linha 5)
                offsetY = 96;  // Linha 5
                if (this->dir.x == 1)      offsetX = 144;  // Direita
                else if (this->dir.x == -1) offsetX = 160; // Esquerda
                else if (this->dir.y == -1) offsetX = 176; // Cima
                else if (this->dir.y == 1)  offsetX = 192; // Baixo
                break;
        }

        this->currentFrame.left = offsetX;
        this->currentFrame.top = offsetY;

    }
    this->sprite.setTextureRect(this->currentFrame);
}

void Ghost::updateAnimationNormal() {
    std::cout << "Hello" << std::endl;
}

void Ghost::updateBehavior() {
    // Comportamento base dos fantasmas
    // Cada fantasma específico pode sobrescrever este comportamento
    switch (this->currentMode) {
        case NORMAL:
            // Movimento normal
            break;
        case POWERLESS:
            // Movimento mais lento ou aleatório
            break;
        case DEAD:
            // Movimento de retorno à posição inicial
            break;
    }
}

void Ghost::setDirection(const sf::Vector2<int>& direction) {
    this->dir = direction;
}

void Ghost::setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize) {
    this->pos = position;
    this->sprite.setPosition(this->pos.x * 1.005*tileSize.x, this->pos.y * 1.005*tileSize.y);
}

void Ghost::setMode(Mode mode) {
    this->currentMode = mode;
}

const sf::Sprite Ghost::getSprite() const {
    return this->sprite;
}

const sf::Vector2<int>& Ghost::getPosition() const {
    return this->pos;
}

Ghost::Mode Ghost::getMode() const {
    return this->currentMode;
}

// Construtor de Blinky que chama o construtor de Ghost
Blinky::Blinky(const std::string& texturePath, int fw, int fh, float fd)
    : Ghost(texturePath, fw, fh, fd) {}

void Blinky::updateAnimationNormal(){
    int offsetY, offsetX;

    // Frames normais para fantasmas
    offsetY = 64;
    if (this->dir.x == -1) offsetX = 32 + (this->currentFrameIndex * this->frameWidth); // Esquerda
    else if (this->dir.y == -1) offsetX = 64 + (this->currentFrameIndex * this->frameWidth); // Cima
    else if (this->dir.y == 1)  offsetX = 96 + (this->currentFrameIndex * this->frameWidth); // Baixo
    else offsetX = 0 + (this->currentFrameIndex * this->frameWidth); // Direita

    this->currentFrame.left = offsetX;
    this->currentFrame.top = offsetY;
    
    this->sprite.setTextureRect(this->currentFrame);
}

// // Construtor de Pinky que chama o construtor de Ghost
Pinky::Pinky(const std::string& texturePath, int fw, int fh, float fd)
     : Ghost(texturePath, fw, fh, fd) {}

 void Pinky::updateAnimationNormal(){
    int offsetY, offsetX;

    // Frames normais para fantasmas
    offsetY = 80;
    if (this->dir.x == -1) offsetX = 32 + (this->currentFrameIndex * this->frameWidth); // Esquerda
    else if (this->dir.y == -1) offsetX = 64 + (this->currentFrameIndex * this->frameWidth); // Cima
    else if (this->dir.y == 1)  offsetX = 96 + (this->currentFrameIndex * this->frameWidth); // Baixo
    else offsetX = 0 + (this->currentFrameIndex * this->frameWidth); // Direita

    this->currentFrame.left = offsetX;
    this->currentFrame.top = offsetY;
     
    this->sprite.setTextureRect(this->currentFrame);
 }

// Construtor de Inky que chama o construtor de Ghost
Inky::Inky(const std::string& texturePath, int fw, int fh, float fd)
    : Ghost(texturePath, fw, fh, fd) {}

void Inky::updateAnimationNormal(){
    int offsetY, offsetX;

    // Frames normais para fantasmas
    offsetY = 96;
    if (this->dir.x == -1) offsetX = 32 + (this->currentFrameIndex * this->frameWidth); // Esquerda
    else if (this->dir.y == -1) offsetX = 64 + (this->currentFrameIndex * this->frameWidth); // Cima
    else if (this->dir.y == 1)  offsetX = 96 + (this->currentFrameIndex * this->frameWidth); // Baixo
    else offsetX = 0 + (this->currentFrameIndex * this->frameWidth); // Direita

    this->currentFrame.left = offsetX;
    this->currentFrame.top = offsetY;
    
    this->sprite.setTextureRect(this->currentFrame);
}

// Construtor de Clyde que chama o construtor de Ghost
Clyde::Clyde(const std::string& texturePath, int fw, int fh, float fd)
    : Ghost(texturePath, fw, fh, fd) {}

void Clyde::updateAnimationNormal(){
    int offsetY, offsetX;

    // Frames normais para fantasmas
    offsetY = 112;
    if (this->dir.x == -1) offsetX = 32 + (this->currentFrameIndex * this->frameWidth); // Esquerda
    else if (this->dir.y == -1) offsetX = 64 + (this->currentFrameIndex * this->frameWidth); // Cima
    else if (this->dir.y == 1)  offsetX = 96 + (this->currentFrameIndex * this->frameWidth); // Baixo
    else offsetX = 0 + (this->currentFrameIndex * this->frameWidth); // Direita

    this->currentFrame.left = offsetX;
    this->currentFrame.top = offsetY;
    
    this->sprite.setTextureRect(this->currentFrame);
}