#ifndef MAZE_HPP
#define MAZE_HPP

#include "core/Core.hpp"
#include <bits/stdc++.h>

class Maze {
  private:
    std::vector<std::vector<Cell>> grid;

  public:
    Maze();

    bool isWalkable(const Position position) const;
    Cell &getCell(const Position position) const;
};

#endif
