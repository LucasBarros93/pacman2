#include "map.hpp"

#include <fstream>
#include <iostream>

Map::Map(const std::string& texturePath, const sf::Vector2f& tileSize)
    : tileSize(tileSize) {
    // if (!this->wallTexture.loadFromFile(texturePath)) {
    //     throw std::runtime_error("Erro ao carregar a textura do mapa!");
    // }
    // this->wallSprite.setTexture(this->wallTexture);

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
    while (std::getline(file, line)) {
        mapData.push_back(line);
    }
    return true;
}

void Map::draw(sf::RenderWindow& window) {
    for (size_t y = 0; y < mapData.size(); y+=2) {
        for (size_t x = 0; x < mapData[y].size(); x+=2) {
            char tile = mapData[y][x];
            char tile2 = mapData[y][x+1];
            sf::Vector2f position(x * this->tileSize.x, y * this->tileSize.y);

            if (tile == '#'){ // Parede
                this->wall.setPosition(position);
                window.draw(this->wall);
            } 
            
            else if (tile == '.' || tile2 == '.'){ // Ponto
                this->dot.setPosition((position.x+this->tileSize.x), (position.y+this->tileSize.y));
                window.draw(this->dot);
            }
            // Outros elementos podem ser adicionados aqui
        }
    }
}

const std::vector<std::string>& Map::getMapData() const {
    return mapData;
}
