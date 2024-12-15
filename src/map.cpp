#include "map.hpp"

#include <fstream>
#include <iostream>

Map::Map(const sf::Vector2<float>& tileSize) : tileSize(tileSize), 
    pac("./assets/images/spritesheet.png", 16, 16, 0.2f),
    blinky("./assets/images/spritesheet.png", 16, 16, 0.2f),
    pinky("./assets/images/spritesheet.png", 16, 16, 0.2f),
    inky("./assets/images/spritesheet.png", 16, 16, 0.2f),
    clyde("./assets/images/spritesheet.png", 16, 16, 0.2f) {

    this->wall.setSize({this->tileSize.x*2, this->tileSize.y*2});
    this->wall.setFillColor(sf::Color::Blue);
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
                    dots.emplace(position, Dot({static_cast<int>(x), static_cast<int>(y)}));
                }
                else if (mapData[y][x] == 'o' || mapData[y][x+1] == 'o') {  // Energizer
                    energizers.emplace(position, Energizer({static_cast<int>(x), static_cast<int>(y)}));
                }
            }
        }
    }

    return true;
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

                        this->wall.setPosition(position);
                        window.draw(this->wall);
                }
            }
            
            if (mapData[y][x] == 'P' &&
                mapData[y+1][x] == 'P' && 
                mapData[y][x+1] == 'P' && 
                mapData[y+1][x+1] == 'P') {

                    this->pac.setPosition({static_cast<int>(x), static_cast<int>(y)}, this->tileSize);
                    window.draw(this->pac.getSprite());
                }
            
            if (mapData[y][x] == 'B' &&
                mapData[y+1][x] == 'B' && 
                mapData[y][x+1] == 'B' && 
                mapData[y+1][x+1] == 'B') {

                    this->blinky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    window.draw(this->blinky.getSprite());
                }
            
            if (mapData[y][x] == 'R' &&
                mapData[y+1][x] == 'R' && 
                mapData[y][x+1] == 'R' && 
                mapData[y+1][x+1] == 'R') {

                    this->pinky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    window.draw(this->pinky.getSprite());
                }

            if (mapData[y][x] == 'I' &&
                mapData[y+1][x] == 'I' && 
                mapData[y][x+1] == 'I' && 
                mapData[y+1][x+1] == 'I') {

                    this->inky.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    window.draw(this->inky.getSprite());
                }

            if (mapData[y][x] == 'C' &&
                mapData[y+1][x] == 'C' && 
                mapData[y][x+1] == 'C' && 
                mapData[y+1][x+1] == 'C') {

                    this->clyde.setPosition({static_cast<int>(x), static_cast<int>(y)},  this->tileSize);
                    window.draw(this->clyde.getSprite());
                }
        }
    }

    // Desenhar frutas (dots e energizers)
    for (const auto& dot : dots) {
        dot.second.draw(window);
    }

    for (const auto& energizer : energizers) {
        energizer.second.draw(window);
    }
}

const std::vector<std::vector <char>>& Map::getMapData() const {
    return mapData;
}

void Map::updatePacman(const sf::Vector2<int> direction){
    this->pac.updateAnimation();
    this->mapData = this->pac.update(this->mapData, direction);
}

void Map::updateGhosts(){
    this->blinky.updateAnimation();
    this->pinky.updateAnimation();
    this->inky.updateAnimation();
    this->clyde.updateAnimation();
}