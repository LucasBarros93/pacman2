#include "maze/Maze.hpp"

Maze::Maze(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo do mapa: " << filePath
                  << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line)) {
        std::vector<char> data;
        std::copy(line.begin(), line.end(), std::back_inserter(data));
    }
}
