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

MapData Ghost::spawn(MapData mapData, char self){
    this->dir = {0,-1};

    char to = mapData[this->pos.y + this->dir.y][this->pos.x];
    /**/
    if(to == ' '){
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y+1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x+1] = ' ';
        mapData[this->pos.y+1][this->pos.x+1] = ' ';

        this->pos += this->dir;

        mapData[this->pos.y][this->pos.x]     = self;
        mapData[this->pos.y+1][this->pos.x]   = self;
        mapData[this->pos.y][this->pos.x+1]   = self;
        mapData[this->pos.y+1][this->pos.x+1] = self;

        return mapData;
    }
    else if (to == '#'){
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y+1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x+1] = ' ';
        mapData[this->pos.y+1][this->pos.x+1] = ' ';

        this->pos = {27,22};

        mapData[this->pos.y][this->pos.x]     = self;
        mapData[this->pos.y+1][this->pos.x]   = self;
        mapData[this->pos.y][this->pos.x+1]   = self;
        mapData[this->pos.y+1][this->pos.x+1] = self;

        this->setMode(NORMAL);

        return mapData;
    }

    return mapData;
}


void Ghost::updateAnimation() {
    if (this->animationClock.getElapsedTime().asSeconds() > this->frameDuration) {
        this->animationClock.restart();
        
        this->currentFrameIndex = (this->currentFrameIndex + 1) % this->frameCount;

        int offsetY = 0;
        int offsetX = 0;

        switch (this->currentMode) {
            case SPAWN:
                // Frames normais para fantasmas
                this->updateAnimationNormal();
                return;

            case NORMAL:
                // Frames normais para fantasmas
                this->updateAnimationNormal();
                return;

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

MapData Ghost::updateBehavior(MapData mapData, char self) {
    this->setMode(SPAWN);
    // Comportamento base dos fantasmas
    // Cada fantasma específico pode sobrescrever este comportamento
    switch (this->currentMode) {
        case SPAWN:
            // Spanwnando
            mapData = this->spawn(mapData, self);
            return mapData;

        case NORMAL:
            // Movimento normal
            this->updateBehaviorNormal(mapData);
            break;
        case POWERLESS:
            // Movimento mais lento ou aleatório
            break;
        case DEAD:
            // Movimento de retorno à posição inicial
            break;
    }

    return mapData;
}

MapData Ghost::updateBehaviorNormal(MapData mapData) {
    return mapData;
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
    int offsetY=0, offsetX=0;

    // Frames normais para fantasmas
    offsetY = 64;
    if (this->dir.x == -1) offsetX = 32 + (this->currentFrameIndex * this->frameWidth); // Esquerda
    else if (this->dir.y == -1) offsetX = 1 + (this->currentFrameIndex * this->frameWidth); // Cima
    else if (this->dir.y == 1)  offsetX = 96 + (this->currentFrameIndex * this->frameWidth); // Baixo
    else offsetX = 0 + (this->currentFrameIndex * this->frameWidth); // Direita


    this->currentFrame.left = offsetX;
    this->currentFrame.top = offsetY;
    
    this->sprite.setTextureRect(this->currentFrame);
}

MapData Blinky::updateBehaviorNormal(MapData mapData) {
    // Possíveis direções para o fantasma
    const std::vector<sf::Vector2<int>> directions = {
        {1, 0},   // Direita
        {-1, 0},  // Esquerda
        {0, 1},   // Baixo
        {0, -1}   // Cima
    };

    // Checa se a direção atual é válida (sem paredes)
    char to1 = '#', to2 = '#';
    if (this->dir.x != 0) {
        to1 = mapData[this->pos.y][this->pos.x + this->dir.x];
        to2 = mapData[this->pos.y + 1][this->pos.x + this->dir.x];
    }
    if (this->dir.y != 0) {
        to1 = mapData[this->pos.y + this->dir.y][this->pos.x];
        to2 = mapData[this->pos.y + this->dir.y][this->pos.x + 1];
    }

    // Se a direção atual estiver bloqueada, escolhe uma nova direção aleatória válida
    if (to1 == '#' || to2 == '#') {
        std::vector<sf::Vector2<int>> validDirections;

        for (const auto& d : directions) {
            char t1 = mapData[this->pos.y + d.y][this->pos.x + d.x];
            char t2 = mapData[this->pos.y + d.y][this->pos.x + d.x + 1];
            char t3 = mapData[this->pos.y + d.y + 1][this->pos.x + d.x];
            char t4 = mapData[this->pos.y + d.y + 1][this->pos.x + d.x + 1];

            if (t1 != '#' && t2 != '#' && t3 != '#' && t4 != '#') {
                validDirections.push_back(d);
            }
        }

        // Escolhe aleatoriamente uma nova direção válida
        if (!validDirections.empty()) {
            int randIndex = rand() % validDirections.size();
            this->setDirection(validDirections[randIndex]);
        } else {
            this->setDirection({0, 0}); // Fantasma para se não houver direção válida
        }
    }

    // Atualiza a posição do fantasma
    sf::Vector2<int> nextPos = this->pos + this->dir;
    mapData[this->pos.y][this->pos.x] = ' '; // Limpa a posição atual

    // Trata teleportação horizontal
    if (nextPos.x < 0) {
        nextPos.x = mapData[0].size() - 2;
    } else if (nextPos.x >= static_cast<int>(mapData[0].size()) - 1) {
        nextPos.x = 0;
    }

    this->pos = nextPos;

    // Atualiza o mapa com a nova posição do fantasma
    mapData[this->pos.y][this->pos.x] = 'B'; // Representa o fantasma no mapa
    mapData[this->pos.y + 1][this->pos.x] = 'B';
    mapData[this->pos.y][this->pos.x + 1] = 'B';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'B';

    return mapData;

}

// Construtor de Pinky que chama o construtor de Ghost
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

MapData Pinky::updateBehaviorNormal(MapData mapData) {
    return mapData;
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

MapData Inky::updateBehaviorNormal(MapData mapData) {
    return mapData;
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

MapData Clyde::updateBehaviorNormal(MapData mapData) {
    return mapData;
}

