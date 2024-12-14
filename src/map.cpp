#include "map.hpp"

#include <fstream>
#include <iostream>

Map::Map(const sf::Vector2<float>& tileSize) : tileSize(tileSize), pac("./assets/images/spritesheet.png", 16, 16, 0.2f) {
    this->wall.setSize({this->tileSize.x*2, this->tileSize.y*2});
    this->wall.setFillColor(sf::Color::Blue);

    // Configuração para os pontos
    this->dot.setRadius(this->tileSize.x/8);
    this->dot.setFillColor(sf::Color::Yellow);
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
        }
    }

    /*
    for (int y = 0; y < mapData.size(); y+=2) {
        for (int x = 0; x < mapData[y].size(); x+=2) {
            char tile = mapData[y][x];
            char tile2 = mapData[y][x+1];
            sf::Vector2<float> position(x * this->tileSize.x, y * this->tileSize.y);

            if (tile == '#'){ // Parede
                this->wall.setPosition(position);
                window.draw(this->wall);
            } 
            else if (tile == '.' || tile2 == '.'){ // Ponto
                this->dot.setPosition((position.x+this->tileSize.x), (position.y+this->tileSize.y));
                window.draw(this->dot);
            }
            if (tile == 'P'){
                this->pac.setPosition({x,y}, this->tileSize);
                window.draw(this->pac.getSprite());
            }
        }
    }
    */
}

const std::vector<std::vector <char>>& Map::getMapData() const {
    return mapData;
}

void Map::updatePacman(const sf::Vector2<int> direction){
    this->pac.updateAnimation();
    this->mapData = this->pac.update(this->mapData, direction);
}