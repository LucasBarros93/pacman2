#ifndef MAZE_HPP
#define MAZE_HPP

#include "core/Core.hpp"
#include <cmath>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

class Maze {
  private:
    std::vector<std::vector<Cell>> grid;

  public:
    Maze(const std::string &filePath);

    bool isWalkable(const GridPosition position) const;
    std::optional<CellType> consumeCollectible(GridPosition position);
    Cell getCell(const GridPosition position) const;
    int getHeight() const;
    int getWidth() const;
};

#endif
