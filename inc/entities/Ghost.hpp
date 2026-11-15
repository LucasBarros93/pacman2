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
    Roaming, ///< Out in the maze, choosing a direction at every tile center.
    Dead,    ///< Eaten: only the eyes are left, heading back to the house.
    Entering ///< The eyes going into the house along a fixed route.
};

/// @brief A ghost.
///
/// Inside the maze a ghost only decides where to go when it passes through
/// the center of a tile, and it never turns back (except in a dead end).
/// Being frightened is separate from the state: a frightened ghost keeps its
/// state and only changes speed, sprite and what happens when it is touched.
///
/// Subclasses give each ghost its own behavior by overriding
/// chooseDirection(). By default a ghost picks randomly.
class Ghost : public Entity {
  private:
    static constexpr float DEFAULT_SPEED =
        6.0f; ///< Tiles per second when roaming.
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

    float frightenedTime = 0.0f; ///< Seconds left of the frightened effect.
    std::vector<Position>
        route; ///< Points to follow while leaving or entering the house.
    std::size_t routeIndex = 0; ///< The next point of @ref route.

    /// @brief The directions the ghost can take at its current tile: free
    ///        ones, except turning back (unless it is a dead end).
    std::vector<Direction> getAvailableDirections(const Maze &maze) const;

    /// @brief Moves in the maze, deciding the next direction at tile centers.
    void updateWalking(const Maze &maze, float deltaTime);

    /// @brief Moves along @ref route in straight lines, ignoring walls.
    /// @return true when the last point was reached.
    bool followRoute(float deltaTime);

    /// @brief The dead ghost reached the door: starts walking into the house.
    void startEntering();

    /// @brief Speed given by the state, before the tunnel limit.
    float stateSpeed() const;

    /// @brief Tells whether the ghost is in the slow part of the tunnel.
    bool inTunnel() const;

  protected:
    /// @brief Picks the next direction at a tile center.
    /// @param maze The maze.
    /// @param options Directions the ghost can take (never empty).
    /// @return One element of @p options.
    virtual Direction chooseDirection(const Maze &maze,
                                      const std::vector<Direction> &options);

    /// @brief Picks the option that gets closest to a target tile.
    ///
    /// Distances are compared in straight line. Ties go to the option that
    /// comes first in @p options.
    /// @param options Directions the ghost can take (never empty).
    /// @param target The tile to approach.
    /// @return One element of @p options.
    Direction chooseTowards(const std::vector<Direction> &options,
                            GridPosition target) const;

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

    /// @brief Back to the starting position, direction and state, and no
    ///        longer frightened.
    void reset() override;

    /// @brief Pac-Man ate the ghost: only the eyes are left, going back to
    ///        the house.
    void eat();

    /// @brief Starts the route out of the house. Does nothing unless the
    ///        ghost is waiting inside.
    void leaveHouse();

    /// @brief Makes the ghost frightened for a while.
    ///
    /// Dead ghosts are not affected. A ghost that is out in the maze turns
    /// around at once.
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
