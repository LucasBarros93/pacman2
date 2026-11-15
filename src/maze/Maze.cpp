#include "maze/Maze.hpp"
#include "core/Core.hpp"

#include <cmath>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace {
/// Converts one character of the map file into a tile.
CellType parseCell(char character, int row, int column) {
    switch (character) {
    case '#':
        return CellType::Wall;
    case '.':
        return CellType::Pellet;
    case 'o':
        return CellType::PowerPellet;
    case ' ':
        return CellType::Empty;
    default:
        throw std::runtime_error(std::string("Invalid character '") +
                                 character + "' in maze at row " +
                                 std::to_string(row) + ", column " +
                                 std::to_string(column));
    }
}
} // namespace

Maze::Maze(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open())
        throw std::runtime_error("Could not open maze file: " + filePath);

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back(); // file saved on Windows

        if (line.empty())
            continue;

        const int row = static_cast<int>(grid.size());

        std::vector<CellType> cells;
        cells.reserve(line.size());

        for (std::size_t column = 0; column < line.size(); ++column)
            cells.push_back(
                parseCell(line[column], row, static_cast<int>(column)));

        if (!grid.empty() && cells.size() != grid.front().size())
            throw std::runtime_error("Maze row " + std::to_string(row) +
                                     " has " + std::to_string(cells.size()) +
                                     " columns, expected " +
                                     std::to_string(grid.front().size()));

        grid.push_back(std::move(cells));
    }

    if (grid.empty())
        throw std::runtime_error("Maze file has no rows: " + filePath);

    initialGrid = grid;
    remainingCollectibles = countCollectibles();
}

int Maze::countCollectibles() const {
    int count = 0;

    for (const auto &row : grid)
        for (const CellType type : row)
            if (type == CellType::Pellet || type == CellType::PowerPellet)
                ++count;

    return count;
}

bool Maze::isInside(GridPosition position) const {
    return position.x >= 0 && position.x < getWidth() && position.y >= 0 &&
           position.y < getHeight();
}

std::optional<CellType> Maze::consumeCollectible(Position position,
                                                 float radius) {
    const int minX = static_cast<int>(std::floor(position.x - radius));
    const int maxX = static_cast<int>(std::ceil(position.x + radius));
    const int minY = static_cast<int>(std::floor(position.y - radius));
    const int maxY = static_cast<int>(std::ceil(position.y + radius));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            if (!isInside({x, y}))
                continue;

            CellType &type = grid[y][x];
            if (type != CellType::Pellet && type != CellType::PowerPellet)
                continue;

            // a pellet sits at the center of its tile, which is exactly (x, y)
            const Position center{static_cast<float>(x), static_cast<float>(y)};
            if (!Collision::overlaps(position, center, radius))
                continue;

            const CellType eaten = type;
            type = CellType::Empty;
            --remainingCollectibles;
            return eaten;
        }
    }

    return std::nullopt;
}

bool Maze::isWalkable(GridPosition position) const {
    return isInside(position) && grid[position.y][position.x] != CellType::Wall;
}

GridPosition Maze::wrap(GridPosition tile) const {
    if (tile.x < 0)
        tile.x = getWidth() - 1;
    else if (tile.x >= getWidth())
        tile.x = 0;

    return tile;
}

bool Maze::canStep(GridPosition from, Direction direction) const {
    return direction != Direction::Neutral &&
           isWalkable(wrap(neighbor(from, direction)));
}

void Maze::reset() {
    grid = initialGrid;
    remainingCollectibles = countCollectibles();
}

int Maze::getRemainingCollectibles() const { return remainingCollectibles; }

CellType Maze::getCellType(GridPosition position) const {
    return isInside(position) ? grid[position.y][position.x] : CellType::Wall;
}

int Maze::getHeight() const { return static_cast<int>(grid.size()); }

int Maze::getWidth() const { return static_cast<int>(grid.front().size()); }
