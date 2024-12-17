#include "entitys/pacman.hpp"

// Construtor da classe Pacman
// Recebe o caminho para a textura do Pacman, bem como sua altura e comprimento
Pacman::Pacman(const std::string& texturePath, int fw, int fh, float fd)
    : pos (27,46), dir(0, 0), frameWidth(fw), frameHeight(fh), frameCount(2),
     frameDuration(fd), currentFrameIndex(0){
    
    // Carrega a spritesheet
    if (!this->texture.loadFromFile(texturePath))
        throw std::runtime_error("Erro ao carregar spritesheet!");
    
    this->sprite.setTexture(this->texture);

    // Configura o primeiro frame da animação (direita)
    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, frameHeight);
    this->sprite.setTextureRect(this->currentFrame);

    this->sprite.setScale(1.f, 1.f); // Ajuste de escala, se necessário
}

// Define a direção atual do PacMan
void Pacman::setDirection(const sf::Vector2<int>& direction) {
    this->dir = direction;
    int auxDir;

    // Determina a linha da spritesheet com base na direção
    if (this->dir.x == 1) // Direita
        auxDir = 0;
    else if (this->dir.x == -1) // Esquerda
        auxDir = 1;
    else if (this->dir.y == 1) // Baixo
        auxDir = 3;
    else if (this->dir.y == -1) // Cima
        auxDir = 2;

    this->currentFrame.top = auxDir * frameHeight; // Atualiza a linha
    this->currentFrameIndex = 0; // Reinicia o frame
    this->currentFrame.left = 0; // Primeiro frame da linha
    this->sprite.setTextureRect(this->currentFrame);

}

// Atualiza a animação do PacMan
void Pacman::updateAnimation() {
    // Se parado
    if (this->dir == sf::Vector2<int>(0, 0)) {
        this->currentFrame.left = 2 * this->frameWidth; // Terceiro frame (boca fechada)
        this->currentFrame.top = 0; // Primeira linha
    }
    // Se andando
    else if (this->animationClock.getElapsedTime().asSeconds() > this->frameDuration) {
        this->animationClock.restart();

        // Avança para o próximo frame
        this->currentFrameIndex = (this->currentFrameIndex + 1) % this->frameCount;
        this->currentFrame.left = this->currentFrameIndex * this->frameWidth;

    }

    this->sprite.setTextureRect(this->currentFrame);
}

// Define a posição do PacMan na tela
void Pacman::setPosition(const sf::Vector2<int>& position, const sf::Vector2<float>& tileSize) {
    this->pos = position;

    // Ajusta a posição do sprite usando os tamanhos dos tiles
    this->sprite.setPosition(this->pos.x * 1.005*tileSize.x, this->pos.y * 1.005*tileSize.y);
}

// Atualiza o mapa com a nova posição do PacMan
MapData Pacman::update(MapData mapData, const sf::Vector2<int> direction) {
    std::vector<sf::Vector2<int>> possibleDirections;

    // Verifica direções válidas (cima, baixo, direita, esquerda)
    if (mapData[this->pos.y + 2][this->pos.x] != '#' && mapData[this->pos.y + 2][this->pos.x + 1] != '#')
        possibleDirections.push_back({0, 1});  // Baixo

    if (mapData[this->pos.y - 1][this->pos.x] != '#' && mapData[this->pos.y - 1][this->pos.x + 1] != '#')
        possibleDirections.push_back({0, -1});  // Cima

    if (mapData[this->pos.y][this->pos.x + 2] != '#' && mapData[this->pos.y + 1][this->pos.x + 2] != '#')
        possibleDirections.push_back({1, 0});  // Direita

    if (mapData[this->pos.y][this->pos.x - 1] != '#' && mapData[this->pos.y + 1][this->pos.x - 1] != '#')
        possibleDirections.push_back({-1, 0});  // Esquerda

    // Verifica se a direção solicitada é válida
    if (std::find(possibleDirections.begin(), possibleDirections.end(), direction) != possibleDirections.end())
        this->setDirection(direction);  // Atualiza a direção
    
    // Se a direção atual não for válida, PacMan para
    if (std::find(possibleDirections.begin(), possibleDirections.end(), this->dir) == possibleDirections.end())
        this->dir = {0,0};  // Atualiza a direção
    

    // Limpa a posição atual do mapa
    mapData[this->pos.y][this->pos.x] = ' ';
    mapData[this->pos.y + 1][this->pos.x] = ' ';
    mapData[this->pos.y][this->pos.x + 1] = ' ';
    mapData[this->pos.y + 1][this->pos.x + 1] = ' ';

    // Atualiza a posição com base na direção
    this->pos += this->dir;

    // Trata teleportação nas bordas horizontais
    if (this->pos.x == -1) {
        this->pos.x = mapData[0].size() - 2; // Teleporta para a borda direita
    } else if (this->pos.x == static_cast<int>(mapData[0].size()) - 1) {
        this->pos.x = 0; // Teleporta para a borda esquerda
    }

    // Atualiza o mapa com a nova posição do PacMan
    mapData[this->pos.y][this->pos.x] = 'P';
    mapData[this->pos.y + 1][this->pos.x] = 'P';
    mapData[this->pos.y][this->pos.x + 1] = 'P';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'P';

    return mapData;
}

// Método para resetar o estado do PacMan para o inicial
void Pacman::reset() {
    this->pos = {27, 46}; // Posição inicial
    this->dir = {0, 0}; // Direção inicial

    this->currentFrameIndex = 0; // Reinicia o índice do frame de animação
    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight); // Frame inicial (direita)

    this->sprite.setTextureRect(this->currentFrame); // Aplica o frame inicial no sprite
    this->sprite.setPosition(this->pos.x * 10, this->pos.y * 10); // Ajusta a posição inicial do sprite na tela
}

// Método para obter o sprite atual do PacMan
const sf::Sprite Pacman::getSprite() const{
    return this->sprite;
}

// Método para obter a posição atual do PacMan no mapa
const sf::Vector2<int>& Pacman::getPosition() const {
    return this->pos;
}

// Método para obter a direção atual do PacMan
const sf::Vector2<int>& Pacman::getDirection() const {
    return this->dir;
}
