#include "entitys/pacman.hpp"

Pacman::Pacman(const std::string& texturePath, int fw, int fh, float fd)
    : pos (27,46), dir(0, 0), frameWidth(fw), frameHeight(fh), frameCount(2),
     frameDuration(fd), currentFrameIndex(0){

    if (!this->texture.loadFromFile(texturePath))
        throw std::runtime_error("Erro ao carregar spritesheet!");
    
    this->sprite.setTexture(this->texture);

    // Configura o primeiro frame (direita)
    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, frameHeight);
    this->sprite.setTextureRect(this->currentFrame);

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
        this->pos.x = mapData[0].size() - 2;
    } else if (this->pos.x == static_cast<int>(mapData[0].size()) - 1) {
        this->pos.x = 0;
    }

    // Atualiza o mapa com a nova posição do Pac-Man
    mapData[this->pos.y][this->pos.x] = 'P';
    mapData[this->pos.y + 1][this->pos.x] = 'P';
    mapData[this->pos.y][this->pos.x + 1] = 'P';
    mapData[this->pos.y + 1][this->pos.x + 1] = 'P';

    return mapData;
}

void Pacman::reset() {
    this->pos = {27, 46};    // Posição inicial
    this->dir = {0, 0};      // Direção inicial
    this->currentFrameIndex = 0;
    this->currentFrame = sf::IntRect(0, 0, this->frameWidth, this->frameHeight);
    this->sprite.setTextureRect(this->currentFrame);
    this->sprite.setPosition(this->pos.x * 10, this->pos.y * 10);
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
