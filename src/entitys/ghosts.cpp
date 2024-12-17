#include "entitys/ghosts.hpp"

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

    sf::Vector2<int> bestDirectionY;
    sf::Vector2<int> bestDirectionX;

    // Determina as direções ideais
    if (this->pos.y < pacmanPos.y) bestDirectionY = {0, 1};   // Baixo
    if (this->pos.y > pacmanPos.y) bestDirectionY = {0, -1};  // Cima

    if (this->pos.x < pacmanPos.x) bestDirectionX = {1, 0};   // Direita
    if (this->pos.x > pacmanPos.x) bestDirectionX = {-1, 0};  // Esquerda

    // Gera um número aleatório entre 0 e 99
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);
    int randomNumber = dis(gen);

    sf::Vector2<int> chosenDirection = {0, 0};
    
    if (randomNumber < this->dificult) { // Fantasma segue o Pac-Man com followChance%
        // Verifica se a direção vertical está disponível
        if (std::find(possibleDirections.begin(), possibleDirections.end(), bestDirectionY) != possibleDirections.end()) 
            chosenDirection = bestDirectionY;
    
        // Caso contrário, usa a direção horizontal
        else if (std::find(possibleDirections.begin(), possibleDirections.end(), bestDirectionX) != possibleDirections.end()) 
            chosenDirection = bestDirectionX;

    }
    else { // Fantasma escolhe uma direção aleatória
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

    // Trata teleportação nas bordas horizontais
    if (this->pos.x == -1) {
        this->pos.x = mapData[0].size() - 2;
    } else if (this->pos.x == static_cast<int>(mapData[0].size()) - 1) {
        this->pos.x = 0;
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

    // Trata teleportação nas bordas horizontais
    if (this->pos.x == -1) {
        this->pos.x = mapData[0].size() - 2;
    } else if (this->pos.x == static_cast<int>(mapData[0].size()) - 1) {
        this->pos.x = 0;
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

    // Trata teleportação nas bordas horizontais
    if (this->pos.x == -1) {
        this->pos.x = mapData[0].size() - 2;
    } else if (this->pos.x == static_cast<int>(mapData[0].size()) - 1) {
        this->pos.x = 0;
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

    // Trata teleportação nas bordas horizontais
    if (this->pos.x == -1) {
        this->pos.x = mapData[0].size() - 2;
    } else if (this->pos.x == static_cast<int>(mapData[0].size()) - 1) {
        this->pos.x = 0;
    }

    // Atualiza a nova posição no mapa
    mapData[this->pos.y][this->pos.x] = 'C';
    mapData[this->pos.y + 1][this->pos.x] = 'C';
    mapData[this->pos.y][this->pos.x + 1] = 'C';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'C';

    return mapData;
}

