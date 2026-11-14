#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "core/Core.hpp"
#include "entities/Entity.hpp"

class Maze;

class Pacman : public Entity {
  private:
    static constexpr float DEFAULT_SPEED = 6.5f;

    float stallTime = 0.0f;
    bool moving = false;

    bool canMove(const Maze &maze, Direction direction) const;

  public:
    explicit Pacman(const Position position);
    bool trySetDirection(Direction newDirection, const Maze &maze);
    void stop();

    bool isMoving() const;
    void stall(float duration);
    void update(const Maze &maze, float deltaTime) override;
    void reset() override;
};

#endif
