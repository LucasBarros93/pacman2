#include "entitys/ghosts.hpp"

Ghost::Ghost(const std::string& texturePath, int fw, int fh, float fd, int df)
    : dir(0, 0), dificult(df), frameWidth(fw), frameHeight(fh), frameCount(2), frameDuration(fd),
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
    if(mapData[this->pos.y+2][this->pos.x] != '#' && mapData[this->pos.y+2][this->pos.x+1] != '#')
        possibleDirections.push_back({0,1});

    if(mapData[this->pos.y-1][this->pos.x] != '#' && mapData[this->pos.y-1][this->pos.x+1] != '#')
        possibleDirections.push_back({0,-1});

    if(mapData[this->pos.y][this->pos.x+2] != '#' && mapData[this->pos.y+1][this->pos.x+2] != '#')
        possibleDirections.push_back({1,0});

    if(mapData[this->pos.y][this->pos.x-1] != '#' && mapData[this->pos.y+1][this->pos.x-1] != '#')
        possibleDirections.push_back({-1,0});

    // Verifica se é uma encruzilhada (mais de 2 direções válidas)
    if (possibleDirections.size() > 1) {
        // Escolhe aleatoriamente uma nova direção válida
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, possibleDirections.size() - 1);

        int randIndex = dis(gen);
        this->dir = possibleDirections[randIndex];  // Nova direção escolhida
    }

    // Atualiza a posição com a direção atual
    sf::Vector2<int> nextPos = this->pos + this->dir;

    // Verifica colisão
    if (mapData[nextPos.y][nextPos.x] != '#') {
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y + 1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x + 1] = ' ';
        mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

        this->pos += this->dir;  // Move o fantasma
    }

    // Atualiza a nova posição no mapa
    mapData[this->pos.y][this->pos.x] = self;
    mapData[this->pos.y+1][this->pos.x] = self;
    mapData[this->pos.y][this->pos.x+1] = self;
    mapData[this->pos.y+1][this->pos.x+1] = self;

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
                offsetY = 64;  // Linha 4 (64px * 2)
                offsetX = 128 + (this->currentFrameIndex * this->frameWidth);
                break;

            case DEAD:
                // Frames "dead" (linha 5)
                offsetY = 80;  // Linha 5
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

MapData Ghost::updateBehavior(MapData mapData, char self, sf::Vector2<int>pacmanPos) {
    // Comportamento base dos fantasmas
    // Cada fantasma específico pode sobrescrever este comportamento
    switch (this->currentMode) {
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
            return mapData;
        
        case DEAD:
            // voltar para o spawn
            break;
            
    }

    return mapData;
}

MapData Ghost::updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) {
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
Blinky::Blinky(const std::string& texturePath, int fw, int fh, float fd, int df)
    : Ghost(texturePath, fw, fh, fd, df) {}

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

MapData Blinky::updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) {
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

    sf::Vector2<int> chosenDirection;

    // Gera um número aleatório entre 0 e 99
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    int randomNumber = dis(gen);

    if (randomNumber < this->dificult) { // Fantasma segue o Pac-Man com followChance%
        sf::Vector2<int> bestDirection = {0, 0};
        int minDistance = std::numeric_limits<int>::max();

        // Calcula a direção ideal (menor distância até o Pac-Man)
        for (const auto& dir : possibleDirections) {
            sf::Vector2<int> nextPos = this->pos + dir;
            int distance = abs(pacmanPos.x - nextPos.x) + abs(pacmanPos.y - nextPos.y); // Distância Manhattan

            if (distance < minDistance) {
                minDistance = distance;
                bestDirection = dir;
            }
        }
        chosenDirection = bestDirection;
    } else { // Fantasma escolhe uma direção aleatória
        std::uniform_int_distribution<> randomDir(0, possibleDirections.size() - 1);
        chosenDirection = possibleDirections[randomDir(gen)];
    }

    // Atualiza a direção e a posição do fantasma
    this->dir = chosenDirection;
    sf::Vector2<int> nextPos = this->pos + this->dir;

    // Verifica colisão
    if (mapData[nextPos.y][nextPos.x] != '#') {
        // Limpa a posição atual
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y + 1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x + 1] = ' ';
        mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

        this->pos += this->dir;  // Move o fantasma
    }

    // Atualiza a nova posição no mapa
    mapData[this->pos.y][this->pos.x] = 'B';
    mapData[this->pos.y + 1][this->pos.x] = 'B';
    mapData[this->pos.y][this->pos.x + 1] = 'B';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'B';

    return mapData;
}

// Construtor de Pinky que chama o construtor de Ghost
Pinky::Pinky(const std::string& texturePath, int fw, int fh, float fd, int df)
     : Ghost(texturePath, fw, fh, fd, df) {}

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

MapData Pinky::updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) {
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
        std::uniform_int_distribution<> dis(0, 99);
        int randomNumber = dis(gen);

        if (randomNumber < this->dificult) { // Fantasma segue o Pac-Man
            sf::Vector2<int> bestDirection = {0, 0};
            int minDistance = std::numeric_limits<int>::max();

            // Calcula a melhor direção para seguir o Pac-Man
            for (const auto& dir : possibleDirections) {
                sf::Vector2<int> nextPos = this->pos + dir;
                int distance = abs(pacmanPos.x - nextPos.x) + abs(pacmanPos.y - nextPos.y);

                if (distance < minDistance) {
                    minDistance = distance;
                    bestDirection = dir;
                }
            }
            this->dir = bestDirection;
        } else { // Escolhe uma direção aleatória
            std::uniform_int_distribution<> randomDir(0, possibleDirections.size() - 1);
            this->dir = possibleDirections[randomDir(gen)];
        }
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

    // Atualiza a nova posição no mapa
    mapData[this->pos.y][this->pos.x] = 'R';
    mapData[this->pos.y + 1][this->pos.x] = 'R';
    mapData[this->pos.y][this->pos.x + 1] = 'R';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'R';

    return mapData;
}

// Construtor de Inky que chama o construtor de Ghost
Inky::Inky(const std::string& texturePath, int fw, int fh, float fd, int df)
    : Ghost(texturePath, fw, fh, fd, df) {}

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

MapData Inky::updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) {
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

    sf::Vector2<int> chosenDirection;

    // Gera um número aleatório entre 0 e 99
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    int randomNumber = dis(gen);

    if (randomNumber < this->dificult) { // Fantasma segue o Pac-Man com followChance%
        sf::Vector2<int> bestDirection = {0, 0};
        int minDistance = std::numeric_limits<int>::max();

        // Calcula a direção ideal (menor distância até o Pac-Man)
        for (const auto& dir : possibleDirections) {
            sf::Vector2<int> nextPos = this->pos + dir;
            int distance = abs(pacmanPos.x - nextPos.x) + abs(pacmanPos.y - nextPos.y); // Distância Manhattan

            if (distance < minDistance) {
                minDistance = distance;
                bestDirection = dir;
            }
        }
        chosenDirection = bestDirection;
    } else { // Fantasma escolhe uma direção aleatória
        std::uniform_int_distribution<> randomDir(0, possibleDirections.size() - 1);
        chosenDirection = possibleDirections[randomDir(gen)];
    }

    // Atualiza a direção e a posição do fantasma
    this->dir = chosenDirection;
    sf::Vector2<int> nextPos = this->pos + this->dir;

    // Verifica colisão
    if (mapData[nextPos.y][nextPos.x] != '#') {
        // Limpa a posição atual
        mapData[this->pos.y][this->pos.x] = ' ';
        mapData[this->pos.y + 1][this->pos.x] = ' ';
        mapData[this->pos.y][this->pos.x + 1] = ' ';
        mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

        this->pos += this->dir;  // Move o fantasma
    }

    // Atualiza a nova posição no mapa
    mapData[this->pos.y][this->pos.x] = 'I';
    mapData[this->pos.y + 1][this->pos.x] = 'I';
    mapData[this->pos.y][this->pos.x + 1] = 'I';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'I';

    return mapData;
}

// Construtor de Clyde que chama o construtor de Ghost
Clyde::Clyde(const std::string& texturePath, int fw, int fh, float fd, int df)
    : Ghost(texturePath, fw, fh, fd, df) {}

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

MapData Clyde::updateBehaviorNormal(MapData mapData, sf::Vector2<int>pacmanPos) {
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
        std::uniform_int_distribution<> dis(0, 99);
        int randomNumber = dis(gen);

        if (randomNumber < this->dificult) { // Fantasma segue o Pac-Man
            sf::Vector2<int> bestDirection = {0, 0};
            int minDistance = std::numeric_limits<int>::max();

            // Calcula a melhor direção para seguir o Pac-Man
            for (const auto& dir : possibleDirections) {
                sf::Vector2<int> nextPos = this->pos + dir;
                int distance = abs(pacmanPos.x - nextPos.x) + abs(pacmanPos.y - nextPos.y);

                if (distance < minDistance) {
                    minDistance = distance;
                    bestDirection = dir;
                }
            }
            this->dir = bestDirection;
        } else { // Escolhe uma direção aleatória
            std::uniform_int_distribution<> randomDir(0, possibleDirections.size() - 1);
            this->dir = possibleDirections[randomDir(gen)];
        }
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

    // Atualiza a nova posição no mapa
    mapData[this->pos.y][this->pos.x] = 'C';
    mapData[this->pos.y + 1][this->pos.x] = 'C';
    mapData[this->pos.y][this->pos.x + 1] = 'C';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'C';

    return mapData;
}

