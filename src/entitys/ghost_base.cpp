#include "entitys/ghost_base.hpp"

Ghost::Ghost(const std::string& texturePath, int fw, int fh, float fd, int df)
    : dir(0, 0), pos(27,26), dificult(df), count(0), frameWidth(fw), frameHeight(fh), frameCount(2), frameDuration(fd),
      currentFrameIndex(0),  currentMode(OUTGAME) {

    if (!this->texture.loadFromFile(texturePath))
        throw std::runtime_error("Erro ao carregar spritesheet dos fantasmas!");
    
    this->sprite.setTexture(this->texture);

    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight);
    this->sprite.setTextureRect(this->currentFrame);

    this->sprite.setScale(1.f, 1.f); // Ajuste de escala, se necessário
}

MapData Ghost::spawn(MapData mapData, char self){
    if(this->count++%5 !=0)
        return mapData;

    this->dir = {0,-1};
    
    char to = mapData[this->pos.y + this->dir.y][this->pos.x];
    
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

MapData Ghost::powerless(MapData mapData, char self){
        std::vector<sf::Vector2<int>> possibleDirections;

    // Verifica direções válidas (baixo, cima, direita, esquerda)
    if (mapData[this->pos.y + 2][this->pos.x] != '#' && mapData[this->pos.y + 2][this->pos.x + 1] != '#')
        possibleDirections.push_back({0, 1});  // Baixo

    if (mapData[this->pos.y - 1][this->pos.x] != '#' && mapData[this->pos.y - 1][this->pos.x + 1] != '#')
        possibleDirections.push_back({0, -1}); // Cima

    if (mapData[this->pos.y][this->pos.x + 2] != '#' && mapData[this->pos.y + 1][this->pos.x + 2] != '#')
        possibleDirections.push_back({1, 0});  // Direita

    if (mapData[this->pos.y][this->pos.x - 1] != '#' && mapData[this->pos.y + 1][this->pos.x - 1] != '#')
        possibleDirections.push_back({-1, 0}); // Esquerda

    // Caso não haja direções válidas, não faz nada
    if (possibleDirections.empty()) {
        return mapData;
    }

    // Identificar corredores retos: 2 direções válidas, sendo uma delas oposta à atual
    if (possibleDirections.size() == 2) {
        for (const auto& dir : possibleDirections) {
            if (dir == -this->dir) { // Direção oposta
                // Mantém a direção atual e não muda
                possibleDirections.erase(std::remove(possibleDirections.begin(), possibleDirections.end(), dir), possibleDirections.end());
                break;
            }
        }
        // Se ainda restar apenas 1 direção válida, continua na mesma direção
        if (possibleDirections.size() == 1) {
            this->dir = possibleDirections[0];
        }
    }

    // Se estiver em uma encruzilhada, escolha nova direção com probabilidade
    if (possibleDirections.size() > 1) {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Escolhe uma direção aleatória
        std::uniform_int_distribution<> randomDir(0, possibleDirections.size() - 1);
        this->dir = possibleDirections[randomDir(gen)];
        
    }

    // Atualiza a posição com a direção atual
    sf::Vector2<int> nextPos = this->pos + this->dir;

    if (mapData[nextPos.y][nextPos.x] != '#') {
        // Limpa a posição atual
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y + 1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x + 1] = ' ';
        mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

        this->pos += this->dir;  // Move o fantasma
    }

    // Trata teleportação nas bordas horizontais
    if (this->pos.x == -1) {
        this->pos.x = mapData[0].size() - 2;
    } else if (this->pos.x == static_cast<int>(mapData[0].size()) - 1) {
        this->pos.x = 0;
    }

    // Atualiza a nova posição no mapa
    mapData[this->pos.y][this->pos.x] = self;
    mapData[this->pos.y + 1][this->pos.x] = self;
    mapData[this->pos.y][this->pos.x + 1] = self;
    mapData[this->pos.y + 1][this->pos.x + 1] = self;

    return mapData;
}

MapData Ghost::kill(MapData mapData, char self){
    int aux = 24;
                
    while(mapData[28][aux] != ' ')
        aux++;
    
    mapData[this->pos.y][this->pos.x] = ' ';
    mapData[this->pos.y + 1][this->pos.x] = ' ';
    mapData[this->pos.y][this->pos.x + 1] = ' ';
    mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

    this->pos = {aux, 28};  // Move o fantasma
    mapData[this->pos.y][this->pos.x] = self;
    mapData[this->pos.y + 1][this->pos.x] = self;
    mapData[this->pos.y][this->pos.x + 1] = self;
    mapData[this->pos.y + 1][this->pos.x + 1] = self;

    this->setMode(DEAD);
    this->dir = {0,0};
    this->count = 0;

    return mapData;
}

void Ghost::updateAnimation() {
    if (this->animationClock.getElapsedTime().asSeconds() > this->frameDuration) {
        this->animationClock.restart();
        
        this->currentFrameIndex = (this->currentFrameIndex + 1) % this->frameCount;

        int offsetY = 0;
        int offsetX = 0;

        switch (this->currentMode) {
            case OUTGAME:
                // Frames normais para fantasmas
                this->updateAnimationNormal();
                return;

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
                offsetY = 64;  // Linha 4 (64px * 2)
                offsetX = 128 + (this->currentFrameIndex * this->frameWidth);
                break;

            case DEAD:
                // Frames "dead" (linha 5)
                offsetY = 80;  // Linha 5
                if (this->dir.x == -1) offsetX = 160; // Esquerda
                else if (this->dir.y == -1) offsetX = 176; // Cima
                else if (this->dir.y == 1)  offsetX = 192; // Baixo
                else                        offsetX = 144;  // Direita
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

MapData Ghost::updateBehavior(MapData mapData, char self, sf::Vector2<int>pacmanPos) {
    // Comportamento base dos fantasmas
    // Cada fantasma específico pode sobrescrever este comportamento
    switch (this->currentMode) {
        case OUTGAME:
            // fora do jogo n se mexe
            return mapData;
        case SPAWN:
            // Spanwnando
            mapData = this->spawn(mapData, self);
            return mapData;

        case NORMAL:
            // Movimento normal
            mapData = this->updateBehaviorNormal(mapData, pacmanPos);
            return mapData;
            
        case POWERLESS:
            // Movimento aleatório
            mapData = this->powerless(mapData, self);
            if(this->count++ > 100){
                this->setMode(NORMAL);
                this->count = 0;
            }

            return mapData;
        
        case DEAD:
            if(this->count++ > 80){
                this->setMode(SPAWN);
                this->count = 0;
            }
            // voltar para o spawn
            break;
            
    }

    return mapData;
}

void Ghost::reset(const sf::Vector2<int>& position) {
    this->pos = position;       // Posição inicial do fantasma
    this->dir = {0, 0};         // Direção inicial
    this->currentMode = OUTGAME; // Modo inicial NORMAL
    this->currentFrameIndex = 0;
    this->sprite.setPosition(this->pos.x * 10, this->pos.y * 10);
}

MapData Ghost::updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) {
    pacmanPos = {0,0};
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

void Ghost::setDificult(int df) {
    this->dificult = df;
}

void Ghost::setCount(int i) {
    this->count = i;
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
