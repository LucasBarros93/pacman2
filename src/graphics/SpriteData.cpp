#include "graphics/SpriteData.hpp"

static_assert(static_cast<int>(GhostType::Blinky) == 0 &&
                  static_cast<int>(GhostType::Clyde) == 3,
              "the ghost rows of the sheet follow the order of GhostType");

static_assert(static_cast<int>(FruitType::Cherry) == 1 &&
                  static_cast<int>(FruitType::Key) == 8,
              "the fruit columns of the sheet follow the order of FruitType");

namespace SpriteData {

namespace {
constexpr int GHOST_FIRST_ROW = 4; ///< Row of Blinky. One row per ghost.
constexpr int GHOST_EXTRA_COLUMN =
    8; ///< Frightened ghosts and eyes start here.

/// The rectangle of the cell at (column, row).
sf::IntRect cell(int column, int row) {
    return sf::IntRect({column * SPRITE_SIZE, row * SPRITE_SIZE},
                       {SPRITE_SIZE, SPRITE_SIZE});
}

/// A rectangle that is not aligned with the cells: it starts at pixel x of
/// the given row and is width pixels wide (used by the 4-digit scores).
sf::IntRect wideCell(int x, int row, int width) {
    return sf::IntRect({x, row * SPRITE_SIZE}, {width, SPRITE_SIZE});
}

/// Order of the directions in the sheet: right, left, up, down. Neutral uses
/// left.
int directionIndex(Direction direction) {
    switch (direction) {
    case Direction::Right:
        return 0;
    case Direction::Up:
        return 2;
    case Direction::Down:
        return 3;
    case Direction::Left:
    case Direction::Neutral:
        break;
    }
    return 1;
}
} // namespace

sf::IntRect pacman(Direction direction, int frame) {
    if (direction == Direction::Neutral)
        return cell(2, 0); // the full circle

    // one row per direction, two frames per row
    return cell(frame, directionIndex(direction));
}

sf::IntRect pacmanLife() { return cell(8, 1); }

sf::IntRect pacmanDeath(int frame) { return cell(frame + 2, 0); }

sf::IntRect ghost(GhostType type, Direction direction, int frame) {
    // two frames per direction, one row per ghost
    return cell(directionIndex(direction) * 2 + frame,
                GHOST_FIRST_ROW + static_cast<int>(type));
}

sf::IntRect ghostFrightened(bool white, int frame) {
    // after the normal sprites, in Blinky's row: blue, then white
    return cell(GHOST_EXTRA_COLUMN + (white ? 2 : 0) + frame, GHOST_FIRST_ROW);
}

sf::IntRect ghostDead(Direction direction) {
    // in Pinky's row, after the normal sprites: one pair of eyes per direction
    return cell(GHOST_EXTRA_COLUMN + directionIndex(direction),
                GHOST_FIRST_ROW + 1);
}

sf::IntRect fruit(FruitType type) {
    // one column per fruit, starting at column 2 (the cherry is 1)
    return cell(1 + static_cast<int>(type), 3);
}

sf::IntRect scorePopup(int points) {
    switch (points) {
    // ghosts: row 8, one cell each
    case 200:
        return cell(0, 8);
    case 400:
        return cell(1, 8);
    case 800:
        return cell(2, 8);
    case 1600:
        return cell(3, 8);

    // fruits up to 700: row 9, one cell each
    case 100:
        return cell(0, 9);
    case 300:
        return cell(1, 9);
    case 500:
        return cell(2, 9);
    case 700:
        return cell(3, 9);

    // the other fruit values have four digits and are wider than a cell
    case 1000:
        return wideCell(4 * SPRITE_SIZE, 9, SPRITE_SIZE + 4);
    case 2000:
        return wideCell(4 * SPRITE_SIZE - 4, 10, SPRITE_SIZE + 8);
    case 3000:
        return wideCell(4 * SPRITE_SIZE - 4, 11, SPRITE_SIZE + 8);
    case 5000:
        return wideCell(4 * SPRITE_SIZE - 4, 12, SPRITE_SIZE + 8);

    default:
        return sf::IntRect();
    }
}

} // namespace SpriteData
