#include "graphics/Renderer.hpp"
#include "entities/Fruit.hpp"
#include "entities/Ghost.hpp"
#include "entities/Pacman.hpp"
#include "game/FruitSpawner.hpp"
#include "game/GameState.hpp"
#include "graphics/SpriteData.hpp"
#include "maze/Maze.hpp"
#include "maze/MazeLayout.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <algorithm>
#include <cstdint>
#include <stdexcept>

namespace {
constexpr float SPRITE_ZOOM =
    1.35f; ///< Sprites are drawn a bit larger than a tile.
constexpr float HUD_ICON_ZOOM =
    1.25f;                           ///< Same, for the icons of the bottom HUD.
constexpr float PELLET_SIZE = 0.10f; ///< Radius of a pellet, in tiles.
constexpr float POWER_PELLET_SIZE =
    0.30f; ///< Radius of a power pellet, in tiles.
constexpr int ANIMATION_INTERVAL_MS = 200; ///< Time between animation frames.
constexpr int FRUITS_IN_HUD =
    7; ///< Fruits of the last rounds shown at the bottom.

/// Scale that draws a sprite a bit larger than a tile of the given size.
float spriteScale(float cellSize) {
    return cellSize / SpriteData::SPRITE_SIZE * SPRITE_ZOOM;
}

/// A view that covers a horizontal band of the window.
sf::View makeBand(float width, float top, float height, float windowHeight) {
    sf::View view(sf::FloatRect({0.f, 0.f}, {width, height}));
    view.setViewport(
        sf::FloatRect({0.f, top / windowHeight}, {1.f, height / windowHeight}));
    return view;
}

/// A copy of the maze image where every colored pixel is white.
sf::Image makeWhiteImage(const sf::Texture &texture) {
    sf::Image image = texture.copyToImage();
    const sf::Vector2u size = image.getSize();

    for (unsigned int y = 0; y < size.y; ++y) {
        for (unsigned int x = 0; x < size.x; ++x) {
            const sf::Color pixel = image.getPixel({x, y});
            const std::uint8_t brightness =
                std::max({pixel.r, pixel.g, pixel.b});
            image.setPixel(
                {x, y}, sf::Color(brightness, brightness, brightness, pixel.a));
        }
    }

    return image;
}
} // namespace

Renderer::Renderer(sf::RenderWindow &window) : window{window} {
    if (!spritesheet.loadFromFile("assets/images/spritesheet.png"))
        throw std::runtime_error("Could not load spritesheet");

    if (!mazeTexture.loadFromFile("assets/images/maze.png"))
        throw std::runtime_error("Could not load maze.png");

    if (!mazeWhiteTexture.loadFromImage(makeWhiteImage(mazeTexture)))
        throw std::runtime_error("Could not create white maze texture");

    if (!font.openFromFile("assets/fonts/pacman.ttf"))
        throw std::runtime_error("Could not load HUD font");

    const sf::Vector2u size = window.getSize();
    const float width = static_cast<float>(size.x);
    const float height = static_cast<float>(size.y);
    const float gameHeight = height - HUD_TOP_HEIGHT - HUD_BOTTOM_HEIGHT;

    hudTopView = makeBand(width, 0.f, HUD_TOP_HEIGHT, height);
    gameView = makeBand(width, HUD_TOP_HEIGHT, gameHeight, height);
    hudBottomView =
        makeBand(width, HUD_TOP_HEIGHT + gameHeight, HUD_BOTTOM_HEIGHT, height);
}

void Renderer::setShowReady(bool show) { showReady = show; }

void Renderer::setShowGameOver(bool show) { showGameOver = show; }

void Renderer::setPaused(bool value) {
    paused = value;
    if (!value)
        animationClock.restart();
}

void Renderer::setMazeFlash(bool flash) { mazeFlash = flash; }

void Renderer::setPacmanDeathFrame(int frame) { pacmanDeathFrame = frame; }

Renderer::MazeTransform Renderer::getMazeTransform(const Maze &maze) const {
    const sf::Vector2f view = gameView.getSize();

    // the largest tile that makes the whole maze fit
    const float cellSize =
        std::min(view.x / maze.getWidth(), view.y / maze.getHeight());

    return {{(view.x - maze.getWidth() * cellSize) / 2.f,
             (view.y - maze.getHeight() * cellSize) / 2.f},
            cellSize};
}

void Renderer::drawSprite(const sf::IntRect &rect, sf::Vector2f position,
                          float scale) {
    sf::Sprite sprite{spritesheet};
    sprite.setTextureRect(rect);
    sprite.setOrigin({rect.size.x / 2.f, rect.size.y / 2.f});
    sprite.setScale({scale, scale});
    sprite.setPosition(position);

    window.draw(sprite);
}

void Renderer::renderMaze(const Maze &maze, const MazeTransform &transform) {
    const sf::Texture &texture = mazeFlash ? mazeWhiteTexture : mazeTexture;
    const sf::Vector2u size = texture.getSize();

    sf::Sprite sprite{texture};
    sprite.setScale({maze.getWidth() * transform.cellSize / size.x,
                     maze.getHeight() * transform.cellSize / size.y});
    sprite.setPosition(transform.offset);

    window.draw(sprite);
}

void Renderer::renderCollectibles(const Maze &maze,
                                  const MazeTransform &transform) {
    // two shapes, reused for every pellet
    sf::CircleShape pellet(transform.cellSize * PELLET_SIZE);
    pellet.setOrigin({pellet.getRadius(), pellet.getRadius()});
    pellet.setFillColor(sf::Color::White);

    sf::CircleShape powerPellet(transform.cellSize * POWER_PELLET_SIZE);
    powerPellet.setOrigin({powerPellet.getRadius(), powerPellet.getRadius()});
    powerPellet.setFillColor(sf::Color::White);

    for (int y = 0; y < maze.getHeight(); ++y) {
        for (int x = 0; x < maze.getWidth(); ++x) {
            const CellType type = maze.getCellType({x, y});

            if (type != CellType::Pellet && type != CellType::PowerPellet)
                continue;

            sf::CircleShape &shape =
                (type == CellType::PowerPellet) ? powerPellet : pellet;

            shape.setPosition(transform.toScreen(
                Position{static_cast<float>(x), static_cast<float>(y)}));
            window.draw(shape);
        }
    }
}

void Renderer::renderFruit(const FruitSpawner &fruitSpawner,
                           const MazeTransform &transform) {
    const auto &fruit = fruitSpawner.getActiveFruit();

    if (!fruit)
        return;

    drawSprite(SpriteData::fruit(fruit->type),
               transform.toScreen(fruit->position),
               spriteScale(transform.cellSize));
}

void Renderer::renderGhosts(const std::vector<std::unique_ptr<Ghost>> &ghosts,
                            const MazeTransform &transform) {
    for (const auto &ghost : ghosts) {
        if (!ghost->isVisible())
            continue;

        sf::IntRect rect;

        if (ghost->isDead())
            rect = SpriteData::ghostDead(ghost->getDirection());
        else if (ghost->isFrightened())
            rect = SpriteData::ghostFrightened(ghost->isFlashingWhite(),
                                               animationFrame);
        else
            rect = SpriteData::ghost(ghost->getType(), ghost->getDirection(),
                                     animationFrame);

        drawSprite(rect, transform.toScreen(ghost->getPosition()),
                   spriteScale(transform.cellSize));
    }
}

void Renderer::renderPacman(const Pacman &pacman,
                            const MazeTransform &transform) {
    // standing still, the mouth stays on frame 0
    const sf::IntRect rect =
        (pacmanDeathFrame >= 0)
            ? SpriteData::pacmanDeath(pacmanDeathFrame)
            : SpriteData::pacman(pacman.getDirection(),
                                 pacman.isMoving() ? animationFrame : 0);

    drawSprite(rect, transform.toScreen(pacman.getPosition()),
               spriteScale(transform.cellSize));
}

void Renderer::renderPopups(const std::vector<ScorePopup> &popups,
                            const MazeTransform &transform) {
    for (const ScorePopup &popup : popups)
        drawSprite(SpriteData::scorePopup(popup.points),
                   transform.toScreen(popup.position),
                   spriteScale(transform.cellSize));
}

void Renderer::renderBanner(const Maze &maze, const MazeTransform &transform,
                            const std::string &str, sf::Color color) {
    sf::Text text{font, str,
                  static_cast<unsigned int>(transform.cellSize * 0.9f)};
    text.setFillColor(color);

    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f});

    text.setPosition(
        {transform.offset.x + (maze.getWidth() / 2.f) * transform.cellSize,
         transform.offset.y +
             (MazeLayout::BANNER_ROW + 0.5f) * transform.cellSize});

    window.draw(text);
}

void Renderer::renderHudTop(const GameState &gameState,
                            const MazeTransform &transform) {
    const float centerX = hudTopView.getSize().x / 2.f;

    sf::Text highScoreLabel{font, "HIGH SCORE", 19};
    sf::Text highScoreValue{font, std::to_string(gameState.getHighScore()), 12};
    sf::Text scoreValue{font, std::to_string(gameState.getScore()), 15};

    // puts the horizontal center of a text at centerX
    const auto placeCentered = [centerX](sf::Text &text, float y) {
        const sf::FloatRect bounds = text.getLocalBounds();
        text.setPosition(
            {centerX - (bounds.position.x + bounds.size.x / 2.f), y});
    };

    placeCentered(highScoreLabel, 2.f);
    placeCentered(highScoreValue, 23.f);
    scoreValue.setPosition({transform.offset.x, 23.f});

    window.draw(highScoreLabel);
    window.draw(highScoreValue);
    window.draw(scoreValue);
}

void Renderer::renderHudBottom(const GameState &gameState, const Maze &maze,
                               const MazeTransform &transform) {
    const float cellSize = transform.cellSize;
    const float scale = cellSize / SpriteData::SPRITE_SIZE * HUD_ICON_ZOOM;
    const float spacing = cellSize * 1.2f;
    const float y = HUD_BOTTOM_HEIGHT / 2.f;

    const float left = transform.offset.x;
    const float right = transform.offset.x + maze.getWidth() * cellSize;

    // spare lives, from the left
    for (int i = 0; i < gameState.getLives(); ++i)
        drawSprite(SpriteData::pacmanLife(),
                   {left + cellSize / 2.f + i * spacing, y}, scale);

    // fruits of the last rounds, from the right (the current one is the last)
    const int round = gameState.getRound();
    const int first = std::max(1, round - (FRUITS_IN_HUD - 1));

    for (int r = first; r <= round; ++r)
        drawSprite(SpriteData::fruit(fruitTypeForRound(r)),
                   {right - cellSize / 2.f - (round - r) * spacing, y}, scale);
}

void Renderer::render(const Pacman &pacman,
                      const std::vector<std::unique_ptr<Ghost>> &ghosts,
                      const std::vector<ScorePopup> &popups,
                      const GameState &gameState,
                      const FruitSpawner &fruitSpawner, const Maze &maze) {
    // The animation only runs while the game does: it stops when paused and
    // while Pac-Man is hidden (the freeze after eating a ghost).
    if (!paused && pacman.isVisible() &&
        animationClock.getElapsedTime().asMilliseconds() >
            ANIMATION_INTERVAL_MS) {
        animationFrame = (animationFrame + 1) % 2;
        animationClock.restart();
    }

    const MazeTransform transform = getMazeTransform(maze);

    window.clear();

    window.setView(hudTopView);
    renderHudTop(gameState, transform);

    window.setView(gameView);
    renderMaze(maze, transform);
    renderCollectibles(maze, transform);
    renderFruit(fruitSpawner, transform);
    if (showReady)
        renderBanner(maze, transform, "READY!", sf::Color::Yellow);
    if (showGameOver)
        renderBanner(maze, transform, "GAME OVER", sf::Color::Red);
    renderGhosts(ghosts, transform);
    if (pacman.isVisible())
        renderPacman(pacman, transform);
    renderPopups(popups, transform);

    window.setView(hudBottomView);
    renderHudBottom(gameState, maze, transform);

    window.display();
}
