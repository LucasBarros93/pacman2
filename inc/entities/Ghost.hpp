#ifndef GHOST_HPP
#define GHOST_HPP

#include "core/Core.hpp"
#include "entities/Entity.hpp"

#include <cstddef>
#include <vector>

class Maze;

/// @brief What a ghost is doing.
enum class GhostState {
    InHouse, ///< Waiting inside the ghost house for its turn to leave.
    Leaving, ///< Walking out of the house along a fixed route.
    Scatter, ///< Out in the maze, heading to its own corner.
    Chase,   ///< Out in the maze, hunting Pac-Man.
    Dead,    ///< Eaten: only the eyes are left, heading back to the house.
    Entering ///< The eyes going into the house along a fixed route.
};

/// @brief A ghost.
///
/// Out in the maze (Scatter or Chase) a ghost decides where to go only at the
/// center of a tile, like in the original game: among the directions it can
/// take (never turning back) it picks the one whose next tile is closest, in a
/// straight line, to its target tile, and ties go to up, left, down, right.
/// It cannot turn up at four tiles of the maze (MazeLayout::NO_UP_TILES).
/// Frightened ghosts choose at random and dead ones head to the house.
///
/// The mode (scatter or chase) is the same for all ghosts and comes from
/// Game, through setMode(). A ghost that is still in the house remembers it
/// and takes it when it leaves.
///
/// Subclasses give each ghost its own target by overriding chooseDirection().
/// By default a ghost picks at random.
class Ghost : public Entity {
  private:
    static constexpr float DEFAULT_SPEED =
        6.0f; ///< Tiles per second in the maze.
    static constexpr float FRIGHTENED_SPEED =
        3.0f; ///< Tiles per second when frightened.
    static constexpr float DEAD_SPEED =
        12.0f; ///< Tiles per second for the eyes.
    static constexpr float TUNNEL_SPEED =
        4.5f; ///< Speed limit inside the tunnel.
    static constexpr float FLASH_TIME =
        2.0f; ///< Seconds before the end when it starts flashing.
    static constexpr float FLASH_INTERVAL =
        0.2f; ///< Seconds between blue and white while flashing.

    GhostType type;   ///< Which ghost it is.
    GhostState state; ///< What it is doing now.
    GhostState
        spawnState; ///< The state it starts in (and returns to on reset).
    GhostMode mode = GhostMode::Scatter; ///< The mode of the game, as last told
                                         ///< by setMode().
    int round = 1; ///< The current round, as last told by setRound().

    float frightenedTime = 0.0f; ///< Seconds left of the frightened effect.
    std::vector<Position>
        route; ///< Points to follow while leaving or entering the house.
    std::size_t routeIndex = 0; ///< The next point of @ref route.

    /// @brief Whether the ghost is out in the maze (scatter or chase).
    [[nodiscard]] bool isOut() const;

    /// @brief The state a ghost takes when it is out, according to @ref mode.
    [[nodiscard]] GhostState outState() const;

    /// @brief The directions the ghost can take at its current tile: free
    ///        ones, except turning back (unless it is a dead end) and, for
    ///        the "no up" tiles, going up.
    std::vector<Direction> getAvailableDirections(const Maze &maze) const;

    /// @brief Moves in the maze, deciding the next direction at tile centers.
    void updateWalking(const Maze &maze, float deltaTime);

    /// @brief Moves along @ref route in straight lines, ignoring walls.
    /// @return true when the last point was reached.
    bool followRoute(float deltaTime);

    /// @brief The dead ghost reached the door: starts walking into the house.
    void startEntering();

    /// @brief Speed given by the state, before the tunnel limit.
    float stateSpeed(const Maze &maze) const;

    /// @brief Tells whether the ghost is in the slow part of the tunnel.
    bool inTunnel() const;

  protected:
    /// @brief Picks the next direction at a tile center, for a ghost that is
    ///        scattering or chasing (not frightened or dead).
    /// @param maze The maze.
    /// @param options Directions the ghost can take (never empty).
    /// @return One element of @p options.
    virtual Direction chooseDirection(const Maze &maze,
                                      const std::vector<Direction> &options);

    /// @brief Speed of a ghost that is out and not frightened.
    /// @param maze The maze.
    virtual float runSpeed(const Maze &maze) const;

    /// @brief Picks the option that gets closest to a target tile.
    ///
    /// Distances are compared in straight line. Ties go to the option that
    /// comes first in @p options.
    /// @param options Directions the ghost can take (never empty).
    /// @param target The tile to approach. It can be outside the maze.
    /// @return One element of @p options.
    Direction chooseTowards(const std::vector<Direction> &options,
                            GridPosition target) const;

    /// @brief Whether the ghost is out and in scatter mode.
    [[nodiscard]] bool isScattering() const;

    /// @brief The current round.
    [[nodiscard]] int getRound() const;

  public:
    /// @param type Which ghost it is.
    /// @param spawnPosition Starting position, in tiles.
    /// @param initialDirection Direction it starts moving in.
    /// @param initialState State it starts in. Ghosts that start inside the
    ///        house wait there until leaveHouse() is called.
    Ghost(GhostType type, Position spawnPosition,
          Direction initialDirection = Direction::Neutral,
          GhostState initialState = GhostState::InHouse);

    /// @brief Moves the ghost for one frame, according to its state.
    void update(const Maze &maze, float deltaTime) override;

    /// @brief Back to the starting position, direction and state, in scatter
    ///        mode and no longer frightened.
    void reset() override;

    /// @brief Tells the ghost the mode of the game.
    ///
    /// A ghost that is out turns around when the mode really changes. One
    /// that is still in the house keeps it for when it leaves.
    /// @param newMode The new mode.
    void setMode(GhostMode newMode);

    /// @brief Tells the ghost which round it is.
    void setRound(int newRound);

    /// @brief Called when Pac-Man loses a life. Does nothing by default.
    virtual void onPacmanDied();

    /// @brief Called when a ghost has just been let out of the house. Does
    ///        nothing by default.
    /// @param releasedType Which ghost it was.
    virtual void onGhostReleased(GhostType releasedType);

    /// @brief Pac-Man ate the ghost: only the eyes are left, going back to
    ///        the house.
    void eat();

    /// @brief Starts the route out of the house. Does nothing unless the
    ///        ghost is waiting inside.
    void leaveHouse();

    /// @brief Makes the ghost frightened for a while.
    ///
    /// Dead ghosts are not affected. A ghost that is out turns around at once.
    /// @param duration How long the effect lasts, in seconds.
    void frighten(float duration);

    [[nodiscard]] bool isInHouse() const;
    [[nodiscard]] bool isLeaving() const;

    /// @brief Whether the frightened effect is active.
    [[nodiscard]] bool isFrightened() const;

    /// @brief Whether it is in the white half of the flashing at the end of
    ///        the frightened effect.
    [[nodiscard]] bool isFlashingWhite() const;

    /// @brief Whether only the eyes are left (dead or entering the house).
    [[nodiscard]] bool isDead() const;

    /// @brief Whether Pac-Man can eat it: frightened and already out in the
    ///        maze.
    [[nodiscard]] bool isEdible() const;

    /// @brief Whether touching it kills Pac-Man.
    [[nodiscard]] bool isDeadly() const;

    [[nodiscard]] GhostType getType() const;
};

#endif
