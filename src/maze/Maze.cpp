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

            case 'o':
                row.push_back({CellType::PowerPellet});
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

std::optional<CellType> Maze::consumeCollectible(Position position,
                                                 float radius) {
    const int minX = static_cast<int>(std::floor(position.first - radius));
    const int maxX = static_cast<int>(std::ceil(position.first + radius));
    const int minY = static_cast<int>(std::floor(position.second - radius));
    const int maxY = static_cast<int>(std::ceil(position.second + radius));

    for (int y = minY; y <= maxY; ++y) {
        if (y < 0 || y >= static_cast<int>(grid.size()))
            continue;

        for (int x = minX; x <= maxX; ++x) {
            if (x < 0 || x >= static_cast<int>(grid[y].size()))
                continue;

            CellType &type = grid[y][x].cellType;
            if (type != CellType::Pellet && type != CellType::PowerPellet)
                continue;

            const Position pelletPosition{static_cast<float>(x),
                                          static_cast<float>(y)};

            if (Collision::overlaps(position, pelletPosition, radius)) {
                const CellType consumed = type;
                type = CellType::Empty;
                return consumed;
            }
        }
    }

    return std::nullopt;
}

bool Maze::isWalkable(const GridPosition position) const {
    const int x = position.first;
    const int y = position.second;

    if (y < 0 || y >= static_cast<int>(grid.size()))
        return false;

    if (x < 0 || x >= static_cast<int>(grid[y].size()))
        return false;

    return grid[y][x].cellType != CellType::Wall;
};

Cell Maze::getCell(const GridPosition position) const {
    return this->grid[position.second][position.first];
}

int Maze::getHeight() const { return static_cast<int>(this->grid.size()); }

int Maze::getWidth() const { return static_cast<int>(this->grid[0].size()); }
