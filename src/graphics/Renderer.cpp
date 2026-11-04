#include "graphics/Renderer.hpp"
#include "entities/Fruit.hpp"
#include "entities/Pacman.hpp"
#include "game/FruitSpawner.hpp"
#include "game/GameState.hpp"
#include "graphics/SpriteData.hpp"
#include "maze/Maze.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>

Renderer::Renderer(sf::RenderWindow &window) : window{window} {
    if (!spritesheet.loadFromFile("assets/images/spritesheet.png"))
        throw std::runtime_error("Could not load spritesheet");

    if (!mazeTexture.loadFromFile("assets/images/maze.png"))
        throw std::runtime_error("Could not load maze.png");

    sf::Image whiteImage = mazeTexture.copyToImage();
    const sf::Vector2u imageSize = whiteImage.getSize();

    for (unsigned int y = 0; y < imageSize.y; ++y) {
        for (unsigned int x = 0; x < imageSize.x; ++x) {
            const sf::Color p = whiteImage.getPixel({x, y});
            const std::uint8_t v = std::max({p.r, p.g, p.b});
            whiteImage.setPixel({x, y}, sf::Color(v, v, v, p.a));
        }
    }

    if (!mazeWhiteTexture.loadFromImage(whiteImage))
        throw std::runtime_error("Could not create white maze texture");

    if (!font.openFromFile("assets/fonts/pacman.ttf"))
        throw std::runtime_error("Could not load HUD font");

    const auto size = window.getSize();

    const float width = static_cast<float>(size.x);
    const float height = static_cast<float>(size.y);

    const float gameHeight =
        height - Layout::HUD_TOP_HEIGHT - Layout::HUD_BOTTOM_HEIGHT;

    hudTopView =
        sf::View(sf::FloatRect({0.f, 0.f}, {width, Layout::HUD_TOP_HEIGHT}));

    gameView = sf::View(sf::FloatRect({0.f, 0.f}, {width, gameHeight}));

    hudBottomView =
        sf::View(sf::FloatRect({0.f, 0.f}, {width, Layout::HUD_BOTTOM_HEIGHT}));

    hudTopView.setViewport(
        sf::FloatRect({0.f, 0.f}, {1.f, Layout::HUD_TOP_HEIGHT / height}));

    gameView.setViewport(sf::FloatRect({0.f, Layout::HUD_TOP_HEIGHT / height},
                                       {1.f, gameHeight / height}));

    hudBottomView.setViewport(
        sf::FloatRect({0.f, (Layout::HUD_TOP_HEIGHT + gameHeight) / height},
                      {1.f, Layout::HUD_BOTTOM_HEIGHT / height}));
}

float Renderer::getMazeScale(const Maze &maze) const {
    const sf::Vector2f viewSize = gameView.getSize();

    const float mazeWidth = maze.getWidth() * SpriteData::CELL_SIZE;

    const float mazeHeight = maze.getHeight() * SpriteData::CELL_SIZE;

    return std::min(viewSize.x / mazeWidth, viewSize.y / mazeHeight);
}

sf::Vector2f Renderer::getMazeOffset(const Maze &maze) const {
    const sf::Vector2f viewSize = gameView.getSize();
    const float scale = getMazeScale(maze);

    const float mazeWidth = maze.getWidth() * SpriteData::CELL_SIZE;

    const float mazeHeight = maze.getHeight() * SpriteData::CELL_SIZE;

    return {(viewSize.x - mazeWidth * scale) / 2.f,
            (viewSize.y - mazeHeight * scale) / 2.f};
}

void Renderer::setShowReady(bool show) { showReady = show; }

void Renderer::setPaused(bool value) {
    paused = value;
    if (!value)
        animationClock.restart();
}

void Renderer::renderPacman(const Pacman &pacman, const Maze &maze) {
    if (!this->paused &&
        animationClock.getElapsedTime().asMilliseconds() > 200) {

        animationFrame = (animationFrame + 1) % 2;
        animationClock.restart();
    }

    sf::Sprite sprite{spritesheet};

    switch (pacman.getDirection()) {
    case Direction::Right:
        sprite.setTextureRect(SpriteData::pacmanRight(animationFrame));
        break;

    case Direction::Left:
        sprite.setTextureRect(SpriteData::pacmanLeft(animationFrame));
        break;

    case Direction::Up:
        sprite.setTextureRect(SpriteData::pacmanUp(animationFrame));
        break;

    case Direction::Down:
        sprite.setTextureRect(SpriteData::pacmanDown(animationFrame));
        break;

    case Direction::Neutral:
        sprite.setTextureRect(SpriteData::pacmanNeutral());
        break;
    }

    const Position position = pacman.getPosition();
    const float scale = getMazeScale(maze);
    const sf::Vector2f offset = getMazeOffset(maze);

    const float cellSize = SpriteData::CELL_SIZE * scale;

    sprite.setScale({(cellSize / SpriteData::SPRITE_SIZE) * 1.35f,
                     (cellSize / SpriteData::SPRITE_SIZE) * 1.35f});

    sprite.setOrigin(
        {SpriteData::SPRITE_SIZE / 2.f, SpriteData::SPRITE_SIZE / 2.f});

    sprite.setPosition({offset.x + (position.x + 0.5f) * cellSize,
                        offset.y + (position.y + 0.5f) * cellSize});

    window.draw(sprite);
}

void Renderer::renderMaze(const Maze &maze) {
    sf::Sprite mazeSprite{mazeFlash ? mazeWhiteTexture : mazeTexture};

    const sf::Vector2u textureSize = mazeTexture.getSize();

    const float targetWidth = maze.getWidth() * SpriteData::CELL_SIZE;

    const float targetHeight = maze.getHeight() * SpriteData::CELL_SIZE;

    const float scale = getMazeScale(maze);
    const sf::Vector2f offset = getMazeOffset(maze);

    mazeSprite.setScale({scale * targetWidth / textureSize.x,
                         scale * targetHeight / textureSize.y});

    mazeSprite.setPosition(offset);

    window.draw(mazeSprite);
}

void Renderer::renderCollectibles(const Maze &maze) {
    const float cellSize = SpriteData::CELL_SIZE * getMazeScale(maze);
    const sf::Vector2f offset = getMazeOffset(maze);

    for (int y = 0; y < maze.getHeight(); ++y) {
        for (int x = 0; x < maze.getWidth(); ++x) {
            const CellType type = maze.getCell({x, y}).cellType;

            if (type != CellType::Pellet && type != CellType::PowerPellet) {
                continue;
            }

            const bool isPowerPellet = type == CellType::PowerPellet;

            const float radius = cellSize * (isPowerPellet ? 0.30f : 0.10f);

            sf::CircleShape pellet(radius);

            pellet.setOrigin({radius, radius});

            pellet.setPosition({offset.x + (x + 0.5f) * cellSize,
                                offset.y + (y + 0.5f) * cellSize});

            pellet.setFillColor(sf::Color::White);
            window.draw(pellet);
        }
    }
}

void Renderer::renderFruit(const FruitSpawner &fruitSpawner, const Maze &maze) {
    const auto &fruit = fruitSpawner.getActiveFruit();

    if (!fruit.has_value())
        return;

    sf::Sprite fruitSprite{spritesheet};

    fruitSprite.setTextureRect(
        SpriteData::fruit(static_cast<int>(fruit->type)));

    const float scale = getMazeScale(maze);
    const float cellSize = SpriteData::CELL_SIZE * scale;
    const sf::Vector2f offset = getMazeOffset(maze);

    fruitSprite.setOrigin(
        {SpriteData::SPRITE_SIZE / 2.f, SpriteData::SPRITE_SIZE / 2.f});

    fruitSprite.setScale({cellSize / SpriteData::SPRITE_SIZE * 1.35f,
                          cellSize / SpriteData::SPRITE_SIZE * 1.35f});

    fruitSprite.setPosition({offset.x + (fruit->position.x + 0.5f) * cellSize,
                             offset.y + (fruit->position.y + 0.5f) * cellSize});

    window.draw(fruitSprite);
}

void Renderer::renderHudTop(const GameState &gameState, const Maze &maze) {
    sf::Text highScoreLabel{font, "HIGH SCORE"};
    highScoreLabel.setCharacterSize(19);
    highScoreLabel.setFillColor(sf::Color::White);

    sf::Text highScoreValue{font, std::to_string(gameState.getHighScore())};
    highScoreValue.setCharacterSize(12);
    highScoreValue.setFillColor(sf::Color::White);

    sf::Text scoreValue{font, std::to_string(gameState.getScore())};
    scoreValue.setCharacterSize(15);
    scoreValue.setFillColor(sf::Color::White);

    const sf::Vector2f viewSize = hudTopView.getSize();
    const float centerX = viewSize.x / 2.f;

    const sf::FloatRect labelBounds = highScoreLabel.getLocalBounds();

    highScoreLabel.setPosition(
        {centerX - (labelBounds.position.x + labelBounds.size.x / 2.f), 2.f});

    const sf::FloatRect highScoreBounds = highScoreValue.getLocalBounds();

    highScoreValue.setPosition(
        {centerX - (highScoreBounds.position.x + highScoreBounds.size.x / 2.f),
         23.f});

    const float margin = getMazeOffset(maze).x;

    scoreValue.setPosition({margin, 23.f});

    window.draw(highScoreLabel);
    window.draw(highScoreValue);
    window.draw(scoreValue);
}

void Renderer::renderHudBottom(const GameState &gameState, const Maze &maze) {
    const float scale = getMazeScale(maze);
    const float margin = getMazeOffset(maze).x;

    const float cellSize = SpriteData::CELL_SIZE * scale;
    const float spriteSize = SpriteData::SPRITE_SIZE;

    const float iconScale = (cellSize / spriteSize) * 1.25f;
    const float spacing = cellSize * 1.2f;

    for (int i = 0; i < gameState.getLives(); ++i) {
        sf::Sprite lifeSprite{spritesheet};

        lifeSprite.setTextureRect(SpriteData::pacmanLife());

        lifeSprite.setScale({iconScale, iconScale});

        lifeSprite.setOrigin({spriteSize / 2.f, spriteSize / 2.f});

        lifeSprite.setPosition({margin + cellSize / 2.f + i * spacing,
                                Layout::HUD_BOTTOM_HEIGHT / 2.f});

        window.draw(lifeSprite);
    }

    const float mazeRight = margin + maze.getWidth() * cellSize;
    const int round = gameState.getRound();
    const int first = std::max(1, round - 6);

    for (int r = first; r <= round; ++r) {
        sf::Sprite icon{spritesheet};
        icon.setTextureRect(
            SpriteData::fruit(static_cast<int>(fruitTypeForRound(r))));
        icon.setScale({iconScale, iconScale});
        icon.setOrigin({spriteSize / 2.f, spriteSize / 2.f});

        const int slot = round - r;
        icon.setPosition({mazeRight - cellSize / 2.f - slot * spacing,
                          Layout::HUD_BOTTOM_HEIGHT / 2.f});

        window.draw(icon);
    }
}

void Renderer::renderReady(const Maze &maze) {
    if (!showReady)
        return;

    const float cellSize = SpriteData::CELL_SIZE * getMazeScale(maze);
    const sf::Vector2f offset = getMazeOffset(maze);

    sf::Text text{font, "READY!", static_cast<unsigned int>(cellSize * 0.9f)};
    text.setFillColor(sf::Color::Yellow);

    const sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f,
                    bounds.position.y + bounds.size.y / 2.f});

    text.setPosition({offset.x + (maze.getWidth() / 2.f) * cellSize,
                      offset.y + (17.0f + 0.5f) * cellSize});

    window.draw(text);
}

void Renderer::render(const Pacman &pacman, const GameState &gameState,
                      const FruitSpawner &fruitSpawner, const Maze &maze) {
    window.clear();

    window.setView(hudTopView);
    renderHudTop(gameState, maze);

    window.setView(gameView);
    renderMaze(maze);
    renderCollectibles(maze);
    renderFruit(fruitSpawner, maze);
    renderReady(maze);
    renderPacman(pacman, maze);

    window.setView(hudBottomView);
    renderHudBottom(gameState, maze);

    window.display();
}

void Renderer::setMazeFlash(bool flash) { mazeFlash = flash; }
