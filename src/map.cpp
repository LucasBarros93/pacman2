#include "map.hpp"

#include <fstream>
#include <iostream>

Map::Map(const std::string& texturePath, const sf::Vector2f& tileSize)
    : tileSize(tileSize) {
    if (!this->wallTexture.loadFromFile(texturePath)) {
        throw std::runtime_error("Erro ao carregar a textura do mapa!");
    }
    this->wallSprite.setTexture(this->wallTexture);

    // Configuração para os pontos
    this->dotShape.setScale(this->tileSize);
    this->dotShape.setFillColor(sf::Color::Yellow);
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
    for (size_t y = 0; y < mapData.size(); ++y) {
        for (size_t x = 0; x < mapData[y].size(); ++x) {
            char tile = mapData[y][x];
            sf::Vector2f position(x * this->tileSize.x, y * this->tileSize.y);

            if (tile == '#') { // Parede
                this->wallSprite.setPosition(position);
                window.draw(this->wallSprite);
            } else if (tile == '.') { // Ponto
                this->dotShape.setPosition(position.x + this->tileSize.x / 3, position.y + this->tileSize.y / 3);
                window.draw(this->dotShape);
            }
            // Outros elementos podem ser adicionados aqui
        }
    }
}

const std::vector<std::string>& Map::getMapData() const {
    return mapData;
}
