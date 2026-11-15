#include "entities/Ghost.hpp"
#include "maze/Maze.hpp"
#include "maze/MazeLayout.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <random>

namespace {
/// Tells whether a movement went through the center of a tile, that is, an
/// integer coordinate in the direction of the movement.
bool crossedTileCenter(Position before, Position after, Direction moved) {
    switch (moved) {
    case Direction::Right:
        return std::floor(after.x) != std::floor(before.x);
    case Direction::Left:
        return std::ceil(after.x) != std::ceil(before.x);
    case Direction::Down:
        return std::floor(after.y) != std::floor(before.y);
    case Direction::Up:
        return std::ceil(after.y) != std::ceil(before.y);
    case Direction::Neutral:
        break;
    }
    return false;
}

std::mt19937 &randomEngine() {
    static std::mt19937 engine{std::random_device{}()};
    return engine;
}

float sign(float value) {
    return value > 0.0f ? 1.0f : (value < 0.0f ? -1.0f : 0.0f);
}
} // namespace

Ghost::Ghost(GhostType type, Position spawnPosition, Direction initialDirection,
             GhostState initialState)
    : Entity(spawnPosition, initialDirection, DEFAULT_SPEED), type{type},
      state{initialState}, spawnState{initialState} {}

void Ghost::reset() {
    Entity::reset();
    state = spawnState;
    route.clear();
    routeIndex = 0;
    frightenedTime = 0.0f;
}

void Ghost::eat() {
    state = GhostState::Dead;
    frightenedTime = 0.0f;
}

GhostType Ghost::getType() const { return type; }

bool Ghost::isInHouse() const { return state == GhostState::InHouse; }

bool Ghost::isLeaving() const { return state == GhostState::Leaving; }

bool Ghost::isDead() const {
    return state == GhostState::Dead || state == GhostState::Entering;
}

bool Ghost::isFrightened() const { return frightenedTime > 0.0f; }

bool Ghost::isEdible() const {
    return isFrightened() && state == GhostState::Roaming;
}

bool Ghost::isDeadly() const { return !isFrightened() && !isDead(); }

bool Ghost::isFlashingWhite() const {
    if (frightenedTime <= 0.0f || frightenedTime > FLASH_TIME)
        return false;

    return static_cast<int>(frightenedTime / FLASH_INTERVAL) % 2 == 1;
}

void Ghost::frighten(float duration) {
    if (isDead())
        return;

    frightenedTime = duration;

    if (state == GhostState::Roaming)
        direction = opposite(direction);
}

void Ghost::leaveHouse() {
    if (state != GhostState::InHouse)
        return;

    state = GhostState::Leaving;
    route = {Position{MazeLayout::HOUSE_CENTER_X, position.y},
             Position{MazeLayout::HOUSE_CENTER_X, MazeLayout::HOUSE_EXIT_Y}};
    routeIndex = 0;
}

void Ghost::startEntering() {
    state = GhostState::Entering;
    route = {Position{MazeLayout::HOUSE_CENTER_X, MazeLayout::HOUSE_EXIT_Y},
             Position{MazeLayout::HOUSE_CENTER_X, MazeLayout::HOUSE_CENTER_Y}};
    routeIndex = 0;
}

Direction Ghost::chooseDirection(const Maze &,
                                 const std::vector<Direction> &options) {
    std::uniform_int_distribution<std::size_t> pick(0, options.size() - 1);
    return options[pick(randomEngine())];
}

Direction Ghost::chooseTowards(const std::vector<Direction> &options,
                               GridPosition target) const {
    const GridPosition tile = getCurrentGridPosition();

    Direction best = options.front();
    int bestDistance = std::numeric_limits<int>::max();

    for (const Direction option : options) {
        const GridPosition next = neighbor(tile, option);
        const int dx = next.x - target.x;
        const int dy = next.y - target.y;
        const int distance = dx * dx + dy * dy;

        if (distance < bestDistance) {
            bestDistance = distance;
            best = option;
        }
    }

    return best;
}

std::vector<Direction> Ghost::getAvailableDirections(const Maze &maze) const {
    // The order breaks ties in chooseTowards (same order as the original game).
    static constexpr std::array<Direction, 4> ALL{
        Direction::Up, Direction::Left, Direction::Down, Direction::Right};

    const GridPosition tile = getCurrentGridPosition();
    std::vector<Direction> options;

    for (const Direction candidate : ALL)
        if (candidate != opposite(direction) && maze.canStep(tile, candidate))
            options.push_back(candidate);

    // dead end: the only way out is to turn back
    if (options.empty())
        options.push_back(opposite(direction));

    return options;
}

float Ghost::stateSpeed() const {
    if (isDead())
        return DEAD_SPEED;
    if (isFrightened())
        return FRIGHTENED_SPEED;
    return DEFAULT_SPEED;
}

bool Ghost::inTunnel() const {
    const GridPosition tile = getCurrentGridPosition();

    return tile.y == MazeLayout::TUNNEL_ROW &&
           (tile.x <= MazeLayout::TUNNEL_LEFT_END ||
            tile.x >= MazeLayout::TUNNEL_RIGHT_START);
}

bool Ghost::followRoute(float deltaTime) {
    float remaining = speed * deltaTime;

    while (remaining > 0.0f && routeIndex < route.size()) {
        const Position target = route[routeIndex];
        const float dx = target.x - position.x;
        const float dy = target.y - position.y;
        const float distance = std::abs(dx) + std::abs(dy); // axis-aligned legs

        // the direction is only used to pick the sprite
        if (dx != 0.0f)
            direction = dx > 0.0f ? Direction::Right : Direction::Left;
        else if (dy != 0.0f)
            direction = dy > 0.0f ? Direction::Down : Direction::Up;

        if (distance <= remaining) {
            position = target;
            remaining -= distance;
            ++routeIndex;
        } else {
            position.x += sign(dx) * remaining;
            position.y += sign(dy) * remaining;
            remaining = 0.0f;
        }
    }

    return routeIndex >= route.size();
}

void Ghost::updateWalking(const Maze &maze, float deltaTime) {
    const Position before = position;
    const Direction moved = direction;

    Entity::update(maze, deltaTime);

    const bool blocked = position.x == before.x && position.y == before.y;

    // only decide when passing through a tile center or when hitting a wall
    if (!blocked && !crossedTileCenter(before, position, moved))
        return;

    if (state == GhostState::Dead &&
        getCurrentGridPosition() == MazeLayout::DEAD_TARGET) {
        startEntering();
        return;
    }

    const std::vector<Direction> options = getAvailableDirections(maze);

    const Direction next = (state == GhostState::Dead)
                               ? chooseTowards(options, MazeLayout::DEAD_TARGET)
                               : chooseDirection(maze, options);

    // when turning (or stopped by a wall), align with the tile center so the
    // ghost does not slide sideways
    if (blocked || next != direction)
        position = {std::round(position.x), std::round(position.y)};

    direction = next;
}

void Ghost::update(const Maze &maze, float deltaTime) {
    if (frightenedTime > 0.0f)
        frightenedTime = std::max(0.0f, frightenedTime - deltaTime);

    // the speed is recomputed every frame from the current state
    speed = stateSpeed();
    if (!isDead() && inTunnel())
        speed = std::min(speed, TUNNEL_SPEED);

    switch (state) {
    case GhostState::InHouse:
        break; // waiting

    case GhostState::Leaving:
        if (followRoute(deltaTime)) {
            state = GhostState::Roaming;
            direction = Direction::Left;
        }
        break;

    case GhostState::Entering:
        if (followRoute(deltaTime)) {
            state = GhostState::InHouse;
            leaveHouse(); // revived: goes straight out again
        }
        break;

    case GhostState::Roaming:
    case GhostState::Dead:
        updateWalking(maze, deltaTime);
        break;
    }
}
