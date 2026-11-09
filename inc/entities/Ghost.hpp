#ifndef GHOST_HPP
#define GHOST_HPP

#include "core/Core.hpp"
#include "entities/Entity.hpp"

#include <cstddef>
#include <vector>

class Maze;

enum class GhostState { InHouse, Leaving, Roaming, Dead, Entering };

class Ghost : public Entity {
  private:
    static constexpr float DEFAULT_SPEED = 6.0f;
    static constexpr float FRIGHTENED_SPEED = 3.0f;
    static constexpr float DEAD_SPEED = 12.0f;

    float frightenedTime = 0.0f;

    GhostType type;
    GhostState state;
    GhostState spawnState;

    std::vector<Position> route;
    std::size_t routeIndex = 0;
    bool visible = true;

    std::vector<Direction> getAvailableDirections(const Maze &maze) const;
    void updateWalking(const Maze &maze, float deltaTime);
    bool followRoute(float deltaTime);
    void startEntering();

  protected:
    virtual Direction chooseDirection(const Maze &maze,
                                      const std::vector<Direction> &options);
    Direction chooseTowards(const std::vector<Direction> &options,
                            GridPosition target) const;

  public:
    Ghost(GhostType type, Position spawnPosition,
          Direction initialDirection = Direction::Neutral,
          GhostState initialState = GhostState::InHouse);

    void update(const Maze &maze, float deltaTime) override;
    void reset() override;

    void eat();
    void leaveHouse();
    void frighten(float duration);
    void respawnInHouse();
    bool isInHouse() const;
    bool isLeaving() const;
    bool isFrightened() const;
    bool isFlashingWhite() const;
    bool isDead() const;
    bool isEdible() const;
    bool isDeadly() const;
    bool isVisible() const;
    GhostType getType() const;
    void setVisible(bool value);
};

#endif
