#ifndef CORE_HPP
#define CORE_HPP

#include <utility>

using Position = std::pair<float, float>;
using GridPosition = std::pair<int, int>;

enum class Direction { Up, Down, Left, Right, Neutral };

enum class CellType {
    Wall,
    Empty,
    Pellet,
    PowerPellet,
};

struct Cell {
    CellType cellType;
};

#endif
