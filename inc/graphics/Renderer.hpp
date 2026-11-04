#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

class FruitSpawner;
class GameState;
class Maze;
class Pacman;

class Renderer {
  private:
    sf::RenderWindow &window;

    sf::Texture spritesheet;
    sf::Texture mazeTexture;
    sf::Texture mazeWhiteTexture;

    bool mazeFlash = false;
    bool showReady = false;
    bool paused = false;

    sf::Font font;

    sf::View hudTopView;
    sf::View gameView;
    sf::View hudBottomView;

    sf::Clock animationClock;
    int animationFrame;

    sf::Vector2f getMazeOffset(const Maze &maze) const;
    float getMazeScale(const Maze &maze) const;

    void renderPacman(const Pacman &pacman, const Maze &maze);
    void renderMaze(const Maze &maze);
    void renderCollectibles(const Maze &maze);
    void renderFruit(const FruitSpawner &fruitSpawner, const Maze &maze);
    void renderHudTop(const GameState &gameState, const Maze &maze);
    void renderHudBottom(const GameState &gameState, const Maze &maze);
    void renderReady(const Maze &maze);

  public:
    Renderer(sf::RenderWindow &window);

    void setShowReady(bool show);
    void setPaused(bool value);

    void render(const Pacman &pacman, const GameState &gameState,
                const FruitSpawner &fruitSpawner, const Maze &maze);

    void setMazeFlash(bool flash);
};

#endif
