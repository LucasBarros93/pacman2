#ifndef GHOST_HPP
#define GHOST_HPP

#include "core/Core.hpp"
#include "entities/Entity.hpp"

#include <cstddef>
#include <vector>

class Maze;

enum class GhostState { InHouse, Leaving, Roaming };

class Ghost : public Entity {
  private:
    static constexpr float DEFAULT_SPEED = 6.0f;
    static constexpr float FRIGHTENED_SPEED = 3.0f;

    float frightenedTime = 0.0f;

    GhostType type;
    GhostState state;
    GhostState spawnState;

    std::vector<Position> route;
    std::size_t routeIndex = 0;

    std::vector<Direction> getAvailableDirections(const Maze &maze) const;
    void updateRoaming(const Maze &maze, float deltaTime);
    void updateLeaving(float deltaTime);

  protected:
    virtual Direction chooseDirection(const Maze &maze,
                                      const std::vector<Direction> &options);

  public:
    Ghost(GhostType type, Position spawnPosition,
          Direction initialDirection = Direction::Neutral,
          GhostState initialState = GhostState::InHouse);

    void update(const Maze &maze, float deltaTime) override;
    void reset() override;

    void leaveHouse();
    void frighten(float duration);
    void respawnInHouse();
    bool isInHouse() const;
    bool isLeaving() const;
    bool isFrightened() const;
    bool isFlashingWhite() const;
    GhostType getType() const;
};

#endif
