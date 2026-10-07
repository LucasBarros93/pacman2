#include "graphics/Renderer.hpp"
#include "graphics/SpriteData.hpp"

Renderer::Renderer(sf::RenderWindow &window) : window{window} {
    if (!spritesheet.loadFromFile("assets/images/spritesheet.png")) {
        throw std::runtime_error("Could not load spritesheet");
    }
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

    sprite.setScale({SpriteData::CELL_SIZE / SpriteData::SPRITE_SIZE,
                     SpriteData::CELL_SIZE / SpriteData::SPRITE_SIZE});
    window.draw(sprite);
}

void Renderer::renderMaze(const Maze &maze) {
    for (int y = 0; y < maze.getHeight(); ++y) {
        for (int x = 0; x < maze.getWidth(); ++x) {

            sf::RectangleShape cell(
                {SpriteData::CELL_SIZE, SpriteData::CELL_SIZE});

            cell.setPosition(
                {x * SpriteData::CELL_SIZE, y * SpriteData::CELL_SIZE});

            // exemplo
            if (maze.getCell({x, y}).cellType == CellType::Wall)
                cell.setFillColor(sf::Color::Blue);
            else
                cell.setFillColor(sf::Color::Black);

            window.draw(cell);
        }
    }
}
