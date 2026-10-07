#include "maze/Maze.hpp"
#include "core/Core.hpp"

Maze::Maze(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open maze file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::vector<Cell> row;

        for (char character : line) {
            switch (character) {
            case '#':
                row.push_back({CellType::Wall});
                break;

            case '.':
                row.push_back({CellType::Pellet});
                break;

            case ' ':
                row.push_back({CellType::Empty});
                break;

            default:
                throw std::runtime_error("Invalid character in maze");
            }
        }

        this->grid.push_back(row);
    }
}

bool Maze::isWalkable(const GridPosition position) const {
    const int x = position.first;
    const int y = position.second;

    if (y < 0 || y >= (int)grid.size())
        return false;

    if (x < 0 || x >= (int)grid[y].size())
        return false;

    return grid[y][x].cellType != CellType::Wall;
};

Cell Maze::getCell(const GridPosition position) const {
    return this->grid[position.second][position.first];
}

int Maze::getHeight() const { return (int)this->grid.size(); }

int Maze::getWidth() const { return (int)this->grid[0].size(); }
