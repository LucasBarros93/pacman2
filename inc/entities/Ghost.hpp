#ifndef GHOST_HPP
#define GHOST_HPP

#include "core/Core.hpp"
#include "entities/Entity.hpp"

#include <vector>

class Maze;

class Ghost : public Entity {
  private:
    static constexpr float DEFAULT_SPEED = 6.0f;

    GhostType type;

    std::vector<Direction> getAvailableDirections(const Maze &maze) const;

  protected:
    bool canEnter(const Maze &maze, GridPosition tile) const override;

    virtual Direction chooseDirection(const Maze &maze,
                                      const std::vector<Direction> &options);

  public:
    Ghost(GhostType type, Position spawnPosition);

    void update(const Maze &maze, float deltaTime) override;
    GhostType getType() const;
};

#endif
