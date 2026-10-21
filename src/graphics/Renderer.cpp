#include "graphics/Renderer.hpp"
#include "graphics/SpriteData.hpp"

Renderer::Renderer(sf::RenderWindow &window) : window{window} {
    if (!spritesheet.loadFromFile("assets/images/spritesheet.png"))
        throw std::runtime_error("Could not load spritesheet");

    if (!mazeTexture.loadFromFile("assets/images/maze.png"))
        throw std::runtime_error("Could not load maze.png");

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

void Renderer::renderPacman(const Pacman &pacman, const Maze &maze) {
    if (animationClock.getElapsedTime().asMilliseconds() > 200) {
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

    sprite.setPosition({offset.x + (position.first + 0.5f) * cellSize,
                        offset.y + (position.second + 0.5f) * cellSize});

    window.draw(sprite);
}

void Renderer::renderMaze(const Maze &maze) {
    sf::Sprite mazeSprite{mazeTexture};

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

void Renderer::renderHudTop(const GameState &gameState, const Maze &maze) {
    sf::Text highScoreLabel{font, "HIGH SCORE"};
    highScoreLabel.setCharacterSize(19);
    highScoreLabel.setFillColor(sf::Color::White);

    sf::Text highScoreValue{font, std::to_string(gameState.getHighScore())};
    highScoreValue.setCharacterSize(12);
    highScoreValue.setFillColor(sf::Color::White);

    sf::Text scoreValue{font, std::to_string(gameState.getScore())};
    scoreValue.setCharacterSize(12);
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

    const float iconScale = (cellSize / spriteSize) * 1.35f;
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
}

void Renderer::render(const Pacman &pacman, const GameState &gameState,
                      const Maze &maze) {
    window.clear();

    window.setView(hudTopView);
    renderHudTop(gameState, maze);

    window.setView(gameView);
    renderMaze(maze);
    renderCollectibles(maze);
    renderPacman(pacman, maze);

    window.setView(hudBottomView);
    renderHudBottom(gameState, maze);

    window.display();
}
