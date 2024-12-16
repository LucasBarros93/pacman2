#include "map.hpp"

Map::Map(const sf::Vector2<float>& tileSize) : tileSize(tileSize), 
    pac("./assets/images/spritesheet.png", 16, 16, 0.2f),
    blinky("./assets/images/spritesheet.png", 16, 16, 0.2f, 50),
    pinky("./assets/images/spritesheet.png", 16, 16, 0.2f, 60),
    inky("./assets/images/spritesheet.png", 16, 16, 0.2f, 70),
    clyde("./assets/images/spritesheet.png", 16, 16, 0.2f, 85) {

    this->wall.setSize({this->tileSize.x*2, this->tileSize.y*2});
    this->wall.setFillColor(sf::Color::Blue);
    // Inicializa mapOffset em zero
    this->mapOffset = sf::Vector2f(0.f, 0.f);
}

bool Map::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo do mapa: " << filePath << std::endl;
        return false;
    }

    mapData.clear();
    std::string line;
    while (std::getline(file, line)){
        std::vector<char> data;
        std::copy(line.begin(), line.end(), std::back_inserter(data));

        mapData.push_back(data);
    }

    for (size_t y = 0; y < mapData.size(); y++) {
        for (size_t x = 0; x < mapData[y].size(); x++) {
            bool isFullTile = (x%2 == 0 && y%2 ==0)? true : false;

            if (isFullTile) {
                std::pair<int, int> position = {static_cast<int>(x), static_cast<int>(y)};

                if (mapData[y][x] == '.' || mapData[y][x+1] == '.') {  // Dot
                    fruits.emplace(position, std::make_unique<Dot>(sf::Vector2<int>(x, y)));
                }
                else if (mapData[y][x] == 'o' || mapData[y][x+1] == 'o') {  // Energizer
                    fruits.emplace(position, std::make_unique<Energizer>(sf::Vector2<int>(x, y)));
                }
            }
        }
    }

    // Calcular o tamanho total do mapa em pixels
    float mapWidthPixels = mapData[0].size() * this->tileSize.x;
    float mapHeightPixels = mapData.size() * this->tileSize.y;

    // Calcula o offset para centralizar o mapa na janela 800x800
    float offsetX = (800.f - mapWidthPixels) / 2.f;
    float offsetY = (800.f - mapHeightPixels) / 2.f;
    if (offsetX < 0) offsetX = 0; // Caso o mapa seja maior que a janela, não deslocar para negativo
    if (offsetY < 0) offsetY = 0;

    this->mapOffset = sf::Vector2f(offsetX, offsetY);


    return true;
}

void Map::reset() {
    // Recarrega o mapa
    this->loadFromFile("assets/maps/map.txt");

    // Reseta Pac-Man
    this->pac.reset();

    // Reseta fantasmas (posições e modos iniciais)
    this->blinky.reset({27, 22});  // Posição inicial
    this->pinky.reset({29, 22});
    this->inky.reset({25, 22});
    this->clyde.reset({31, 22});

    // Limpa frutas
    this->fruits.clear();
}

void Map::draw(sf::RenderWindow& window) {
    bool isFullTile;
    for (size_t y = 0; y < mapData.size(); y++) {
        for (size_t x = 0; x < mapData[y].size(); x++) {
            sf::Vector2<float> position(x * this->tileSize.x, y * this->tileSize.y);
            isFullTile = (x%2 == 0 && y%2 ==0)? true : false;

            if (isFullTile) {
                if (mapData[y][x] == '#' &&
                    mapData[y+1][x] == '#' && 
                    mapData[y][x+1] == '#' && 
                    mapData[y+1][x+1] == '#') {

                        this->wall.setPosition(position + this->mapOffset);
                        window.draw(this->wall);
                }
            }
            
            if (mapData[y][x] == 'P' &&
                mapData[y+1][x] == 'P' && 
                mapData[y][x+1] == 'P' && 
                mapData[y+1][x+1] == 'P') {

                    this->pac.setPosition({static_cast<int>(x), static_cast<int>(y)}, this->tileSize);
                    // Cria uma cópia do sprite do Pac-Man, ajusta posição, depois desenha
                    sf::Sprite pacSprite = this->pac.getSprite();
                    pacSprite.setPosition(pacSprite.getPosition() + this->mapOffset);
                    window.draw(pacSprite);


            }
            
            if (mapData[y][x] == 'B' &&
                mapData[y+1][x] == 'B' && 
                mapData[y][x+1] == 'B' && 
                mapData[y+1][x+1] == 'B') {

                    this->blinky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    sf::Sprite blinkySprite = this->blinky.getSprite();
                    blinkySprite.setPosition(blinkySprite.getPosition() + this->mapOffset);
                    window.draw(blinkySprite);
            }
            
            if (mapData[y][x] == 'R' &&
                mapData[y+1][x] == 'R' && 
                mapData[y][x+1] == 'R' && 
                mapData[y+1][x+1] == 'R') {

                    this->pinky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    sf::Sprite pinkySprite = this->pinky.getSprite();
                    pinkySprite.setPosition(pinkySprite.getPosition() + this->mapOffset);
                    window.draw(pinkySprite);
                }

            if (mapData[y][x] == 'I' &&
                mapData[y+1][x] == 'I' && 
                mapData[y][x+1] == 'I' && 
                mapData[y+1][x+1] == 'I') {

                    this->inky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    sf::Sprite inkySprite = this->inky.getSprite();
                    inkySprite.setPosition(inkySprite.getPosition() + this->mapOffset);
                    window.draw(inkySprite);
                }

            if (mapData[y][x] == 'C' &&
                mapData[y+1][x] == 'C' && 
                mapData[y][x+1] == 'C' && 
                mapData[y+1][x+1] == 'C') {

                    this->clyde.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    sf::Sprite clydeSprite = this->clyde.getSprite();
                    clydeSprite.setPosition(clydeSprite.getPosition() + this->mapOffset);
                    window.draw(clydeSprite);
                }
        }
    }

    // Desenhar frutas (dots e energizers)
    for (const auto& fruitPair : fruits) {
        fruitPair.second->draw(window, this->mapOffset);
    }
}

const MapData& Map::getMapData() const {
    return this->mapData;
}

int Map::updatePacman(const sf::Vector2<int> direction) {
    int pointsEarned = 0;

    // Atualiza a posição do Pac-Man
    this->pac.updateAnimation();
    this->mapData = this->pac.update(this->mapData, direction);

    // Verifica se o Pac-Man coletou uma fruta
    auto it = this->fruits.find({pac.getPosition().x, pac.getPosition().y});
    if (it != this->fruits.end()) {
        pointsEarned = it->second->getPoints(); // Pontos da fruta
        this->fruits.erase(it); // Remove a fruta
    }

    return pointsEarned;
}

void Map::updateGhosts(){
    this->blinky.updateAnimation();
    // this->mapData = this->blinky.updateBehavior(this->mapData, 'B', this->pac.getPosition());
    this->pinky.updateAnimation();
    // this->mapData = this->pinky.updateBehavior(this->mapData, 'R', this->pac.getPosition());
    this->inky.updateAnimation();
    // this->mapData = this->inky.updateBehavior(this->mapData, 'I', this->pac.getPosition());
    this->clyde.updateAnimation();
    // this->mapData = this->clyde.updateBehavior(this->mapData, 'C', this->pac.getPosition());
}

bool Map::colision(){
    if(this->pac.getPosition() == this->clyde.getPosition()){
        if(this->clyde.getMode() == Ghost::Mode::POWERLESS){
            this->mapData = this->clyde.kill(this->mapData, 'C');
            return false;
        }
        else if(this->clyde.getMode() == Ghost::Mode::NORMAL){
            return true;
        }
    }
    if(this->pac.getPosition() == this->inky.getPosition()){
        if(this->inky.getMode() == Ghost::Mode::POWERLESS){
            this->mapData = this->inky.kill(this->mapData, 'I');
            return false;
        }
        else if(this->inky.getMode() == Ghost::Mode::NORMAL){
            return true;
        }
    }
    return false;
}

int Map::getFruitsRemaining() const {
    return fruits.size();
}