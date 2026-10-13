#include "graphics/Renderer.hpp"
#include "graphics/SpriteData.hpp"

Renderer::Renderer(sf::RenderWindow &window) : window{window} {
    if (!spritesheet.loadFromFile("assets/images/spritesheet.png"))
        throw std::runtime_error("Could not load spritesheet");

    if (!mazeTexture.loadFromFile("assets/images/maze.png"))
        throw std::runtime_error("Could not load maze.png");
};

void Renderer::renderPacman(const Pacman &pacman) {
    if (this->animationClock.getElapsedTime().asMilliseconds() > 200) {
        this->animationFrame = (this->animationFrame + 1) % 2;
        this->animationClock.restart();
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

    Position position = pacman.getPosition();
    sprite.setPosition({position.first * SpriteData::CELL_SIZE,
                        position.second * SpriteData::CELL_SIZE});

    sprite.setScale({(SpriteData::CELL_SIZE / SpriteData::SPRITE_SIZE) * 1.35,
                     (SpriteData::CELL_SIZE / SpriteData::SPRITE_SIZE) * 1.35});
    window.draw(sprite);
}

void Renderer::renderMaze(const Maze &maze) {
    sf::Sprite mazeSprite{mazeTexture};

    const sf::Vector2u textureSize = mazeTexture.getSize();

    const float targetWidth = maze.getWidth() * SpriteData::CELL_SIZE;

    const float targetHeight = maze.getHeight() * SpriteData::CELL_SIZE;

    const float scaleX = targetWidth / textureSize.x;
    const float scaleY = targetHeight / textureSize.y;

    mazeSprite.setScale({scaleX, scaleY});
    mazeSprite.setPosition({0.f, 0.f});

    window.draw(mazeSprite);
}

void Renderer::renderCollectibles(const Maze &maze) {
    for (int y = 0; y < maze.getHeight(); ++y) {
        for (int x = 0; x < maze.getWidth(); ++x) {
            const CellType type = maze.getCell({x, y}).cellType;

            if (type != CellType::Pellet && type != CellType::PowerPellet) {
                continue;
            }

            const bool isPowerPellet = type == CellType::PowerPellet;

            const float radius = isPowerPellet ? 6.f : 2.5f;
            sf::CircleShape pellet(radius);

            pellet.setOrigin({radius, radius});
            pellet.setPosition({(x + 0.5f) * SpriteData::CELL_SIZE,
                                (y + 0.5f) * SpriteData::CELL_SIZE});

            pellet.setFillColor(sf::Color::White);
            window.draw(pellet);
        }
    }
}
