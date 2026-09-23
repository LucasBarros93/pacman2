#ifndef CORE_HPP
#define CORE_HPP

#include <bits/stdc++.h>

typedef std::pair<float, float> Position;

enum class Direction { Up, Down, Left, Right, Neutral };

enum class CellType {
    Wall,
    Empty,
    Pellet,
    PowerPellet,
};

typedef struct cell {
    CellType cellType;
} Cell;

#endif
