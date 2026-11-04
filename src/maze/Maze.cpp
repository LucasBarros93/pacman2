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
    initialGrid = grid;
    remainingCollectibles = countCollectibles();
}

int Maze::countCollectibles() const {
    int count = 0;
    for (const auto &row : grid)
        for (const Cell &cell : row)
            if (cell.cellType == CellType::Pellet ||
                cell.cellType == CellType::PowerPellet)
                ++count;
    return count;
}

std::optional<CellType> Maze::consumeCollectible(Position position,
                                                 float radius) {
    const int minX = static_cast<int>(std::floor(position.x - radius));
    const int maxX = static_cast<int>(std::ceil(position.x + radius));
    const int minY = static_cast<int>(std::floor(position.y - radius));
    const int maxY = static_cast<int>(std::ceil(position.y + radius));

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
                --remainingCollectibles;
                return consumed;
            }
        }
    }

    return std::nullopt;
}

bool Maze::isWalkable(const GridPosition position) const {
    if (position.y < 0 || position.y >= static_cast<int>(grid.size()))
        return false;

    if (position.x < 0 ||
        position.x >= static_cast<int>(grid[position.y].size()))
        return false;

    return grid[position.y][position.x].cellType != CellType::Wall;
};

void Maze::reset() {
    grid = initialGrid;
    remainingCollectibles = countCollectibles();
}

int Maze::getRemainingCollectibles() const { return remainingCollectibles; }

Cell Maze::getCell(const GridPosition position) const {
    const int x = position.x;
    const int y = position.y;

    if (y < 0 || y >= static_cast<int>(grid.size()) || x < 0 ||
        x >= static_cast<int>(grid[y].size()))
        return Cell{CellType::Wall};

    return grid[y][x];
}

int Maze::getHeight() const { return static_cast<int>(this->grid.size()); }

int Maze::getWidth() const { return static_cast<int>(this->grid[0].size()); }
