#include "entities/Ghost.hpp"
#include "maze/Maze.hpp"

#include <array>
#include <cmath>
#include <limits.h>
#include <random>

namespace {

constexpr float FLASH_TIME = 2.0f;
constexpr float FLASH_INTERVAL = 0.2f;
constexpr float HOUSE_CENTER_Y = 14.0f;
constexpr float HOUSE_CENTER_X = 13.5f;
constexpr float HOUSE_EXIT_Y = 11.0f;
constexpr GridPosition DEAD_TARGET{13, 11};
constexpr int TUNNEL_ROW = 14;
constexpr int TUNNEL_LEFT_END = 5;
constexpr int TUNNEL_RIGHT_START = 22;

GridPosition neighbor(GridPosition tile, Direction direction) {
    switch (direction) {
    case Direction::Up:
        return {tile.x, tile.y - 1};
    case Direction::Down:
        return {tile.x, tile.y + 1};
    case Direction::Left:
        return {tile.x - 1, tile.y};
    case Direction::Right:
        return {tile.x + 1, tile.y};
    case Direction::Neutral:
        break;
    }
    return tile;
}

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
    speed = DEFAULT_SPEED;
}

void Ghost::eat() {
    state = GhostState::Dead;
    frightenedTime = 0.0f;
    speed = DEAD_SPEED;
}

GhostType Ghost::getType() const { return type; }

bool Ghost::isInHouse() const { return state == GhostState::InHouse; }

bool Ghost::isLeaving() const { return state == GhostState::Leaving; }

bool Ghost::isDead() const {
    return state == GhostState::Dead || state == GhostState::Entering;
}

bool Ghost::isEdible() const {
    return isFrightened() && state == GhostState::Roaming;
}

bool Ghost::isDeadly() const { return !isFrightened() && !isDead(); }

Direction Ghost::chooseDirection(const Maze &,
                                 const std::vector<Direction> &options) {
    std::uniform_int_distribution<std::size_t> pick(0, options.size() - 1);
    return options[pick(randomEngine())];
}

std::vector<Direction> Ghost::getAvailableDirections(const Maze &maze) const {
    static constexpr std::array<Direction, 4> ALL{
        Direction::Up, Direction::Left, Direction::Down, Direction::Right};

    const GridPosition tile = getCurrentGridPosition();
    std::vector<Direction> options;

    for (const Direction candidate : ALL) {
        if (candidate == opposite(direction))
            continue;

        const GridPosition target =
            teleportHorizontal(maze, neighbor(tile, candidate));

        if (!maze.isWalkable(target))
            continue;

        if (candidate == Direction::Down &&
            maze.getCell(target).cellType == CellType::GhostDoor)
            continue;

        options.push_back(candidate);
    }

    if (options.empty())
        options.push_back(opposite(direction));

    return options;
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

void Ghost::leaveHouse() {
    if (state != GhostState::InHouse)
        return;

    state = GhostState::Leaving;
    route = {Position{HOUSE_CENTER_X, position.y},
             Position{HOUSE_CENTER_X, HOUSE_EXIT_Y}};
    routeIndex = 0;
}

void Ghost::frighten(float duration) {
    if (isDead())
        return;

    frightenedTime = duration;
    speed = FRIGHTENED_SPEED;

    if (state == GhostState::Roaming)
        direction = opposite(direction);
}

bool Ghost::isFrightened() const { return frightenedTime > 0.0f; }

bool Ghost::isFlashingWhite() const {
    if (frightenedTime <= 0.0f || frightenedTime > FLASH_TIME)
        return false;

    return static_cast<int>(frightenedTime / FLASH_INTERVAL) % 2 == 1;
}

void Ghost::respawnInHouse() {
    position = {HOUSE_CENTER_X, HOUSE_CENTER_Y};
    direction = Direction::Neutral;
    frightenedTime = 0.0f;
    speed = DEFAULT_SPEED;
    state = GhostState::InHouse;
    leaveHouse();
}

bool Ghost::followRoute(float deltaTime) {
    float remaining = speed * deltaTime;

    while (remaining > 0.0f && routeIndex < route.size()) {
        const Position target = route[routeIndex];
        const float dx = target.x - position.x;
        const float dy = target.y - position.y;
        const float distance = std::abs(dx) + std::abs(dy);

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

    if (routeIndex >= route.size()) {
        state = GhostState::Roaming;
        direction = Direction::Left;
    }

    return routeIndex >= route.size();
}

void Ghost::startEntering() {
    state = GhostState::Entering;
    route = {Position{HOUSE_CENTER_X, HOUSE_EXIT_Y},
             Position{HOUSE_CENTER_X, HOUSE_CENTER_Y}};
    routeIndex = 0;
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

    return tile.y == TUNNEL_ROW &&
           (tile.x <= TUNNEL_LEFT_END || tile.x >= TUNNEL_RIGHT_START);
}

void Ghost::updateWalking(const Maze &maze, float deltaTime) {
    if (direction == Direction::Neutral)
        return;

    const Position before = position;
    const Direction moved = direction;

    Entity::update(maze, deltaTime);

    const bool blocked = position.x == before.x && position.y == before.y;

    if (!blocked && !crossedTileCenter(before, position, moved))
        return;

    const std::vector<Direction> options = getAvailableDirections(maze);

    if (state == GhostState::Dead) {
        const GridPosition tile = getCurrentGridPosition();

        if (tile.x == DEAD_TARGET.x && tile.y == DEAD_TARGET.y) {
            startEntering();
            return;
        }
    }

    const Direction next = (state == GhostState::Dead)
                               ? chooseTowards(options, DEAD_TARGET)
                               : chooseDirection(maze, options);

    if (blocked || next != direction)
        position = {std::round(position.x), std::round(position.y)};

    direction = next;
}

void Ghost::update(const Maze &maze, float deltaTime) {
    if (frightenedTime > 0.0f)
        frightenedTime = std::max(0.0f, frightenedTime - deltaTime);

    speed = stateSpeed();
    if (!isDead() && inTunnel())
        speed = std::min(speed, TUNNEL_SPEED);

    switch (state) {
    case GhostState::InHouse:
        break;

    case GhostState::Leaving:
        if (followRoute(deltaTime)) {
            state = GhostState::Roaming;
            direction = Direction::Left;
        }
        break;

    case GhostState::Entering:
        if (followRoute(deltaTime)) {
            speed = DEFAULT_SPEED;
            state = GhostState::InHouse;
            leaveHouse();
        }
        break;

    case GhostState::Roaming:
    case GhostState::Dead:
        updateWalking(maze, deltaTime);
        break;
    }
}
