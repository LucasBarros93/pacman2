#ifndef MAZE_HPP
#define MAZE_HPP

#include "core/Core.hpp"

#include <optional>
#include <string>
#include <vector>

/// @brief The maze grid: walls, pellets and which tiles can be walked on.
///
/// The maze is loaded once from a text file where each character is a tile:
/// `#` wall, `.` pellet, `o` power pellet and a space for an empty tile.
/// All rows must have the same length. The pellets eaten during a round can
/// be put back with reset().
class Maze {
  private:
    std::vector<std::vector<CellType>>
        grid; ///< Current tiles, indexed [row][column].
    std::vector<std::vector<CellType>>
        initialGrid; ///< Tiles right after loading, used by reset().
    int remainingCollectibles =
        0; ///< Pellets and power pellets still in the maze.

    /// @brief Counts the pellets and power pellets currently in the grid.
    int countCollectibles() const;

    /// @brief Tells whether a tile is inside the grid.
    bool isInside(GridPosition position) const;

  public:
    /// @brief Loads a maze from a text file.
    /// @param filePath Path of the map file.
    /// @throws std::runtime_error If the file cannot be opened, has no rows,
    ///         has a character that is not a tile, or has rows of different
    ///         lengths. Blank lines are ignored.
    explicit Maze(const std::string &filePath);

    /// @brief Tells whether Pac-Man and the ghosts can step on a tile.
    /// @param position The tile to check.
    /// @return false for walls and for anything outside the maze.
    [[nodiscard]] bool isWalkable(GridPosition position) const;

    /// @brief Brings a tile that left through a side of the maze to the
    ///        opposite side (the tunnel).
    /// @param tile Any tile. Only its column can change.
    /// @return @p tile if its column is inside the maze, otherwise the tile
    ///         on the opposite edge.
    [[nodiscard]] GridPosition wrap(GridPosition tile) const;

    /// @brief Tells whether Pac-Man or a ghost can take one step.
    /// @param from The tile where the step starts.
    /// @param direction Where to step. Direction::Neutral never moves.
    /// @return true if the tile reached (going through the tunnel when
    ///         needed) is walkable.
    [[nodiscard]] bool canStep(GridPosition from, Direction direction) const;

    /// @brief Eats the pellet near a position, if there is one.
    /// @param position Where Pac-Man is, in tiles.
    /// @param radius How close the position must be to the center of the
    ///        pellet's tile.
    /// @return The kind of pellet that was eaten, or std::nullopt if none was
    ///         within reach.
    [[nodiscard]] std::optional<CellType>
    consumeCollectible(Position position,
                       float radius = Collision::PELLET_RADIUS);

    /// @brief Puts back every pellet that was eaten (used at a new round).
    void reset();

    /// @brief Pellets and power pellets that are still in the maze.
    /// @return The number left. The round is over when it reaches 0.
    [[nodiscard]] int getRemainingCollectibles() const;

    /// @brief What a tile contains.
    /// @param position The tile to read.
    /// @return Its content, or CellType::Wall if @p position is outside the
    ///         maze.
    [[nodiscard]] CellType getCellType(GridPosition position) const;

    /// @brief Number of rows.
    [[nodiscard]] int getHeight() const;

    /// @brief Number of columns.
    [[nodiscard]] int getWidth() const;
};

#endif
