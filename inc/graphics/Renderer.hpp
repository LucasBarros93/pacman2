#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "core/Core.hpp"
#include "game/ScorePopup.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

#include <memory>
#include <string>
#include <vector>

class FruitSpawner;
class GameState;
class Ghost;
class Maze;
class Pacman;

/// @brief Draws the game: maze, pellets, fruit, characters and HUD.
///
/// The window is split into three horizontal bands, each with its own view:
/// the score on top, the maze in the middle and the lives and fruits at the
/// bottom. The maze is drawn as large as fits in the middle band, centered.
/// The Renderer only draws: it never changes the game, and the game tells it
/// what to show through the setters.
class Renderer {
  private:
    static constexpr float HUD_TOP_HEIGHT =
        40.f; ///< Height of the top band, in pixels.
    static constexpr float HUD_BOTTOM_HEIGHT =
        30.f; ///< Height of the bottom band, in pixels.

    /// @brief Where the maze is on the screen.
    struct MazeTransform {
        sf::Vector2f offset; ///< Top-left corner of the maze in the game view,
                             ///< in pixels.
        float cellSize;      ///< Side of a tile, in pixels.

        /// @brief Pixel position of the center of a position given in tiles.
        sf::Vector2f toScreen(Position position) const {
            return {offset.x + (position.x + 0.5f) * cellSize,
                    offset.y + (position.y + 0.5f) * cellSize};
        }
    };

    sf::RenderWindow &window;

    sf::Texture spritesheet; ///< assets/images/spritesheet.png
    sf::Texture mazeTexture; ///< The maze image.
    sf::Texture
        mazeWhiteTexture; ///< The same image in white, for the flashing.
    sf::Font font;

    sf::View hudTopView;    ///< Top band: score.
    sf::View gameView;      ///< Middle band: the maze and what moves in it.
    sf::View hudBottomView; ///< Bottom band: lives and fruits.

    sf::Clock animationClock; ///< Times the animation frames.
    int animationFrame = 0;   ///< 0 or 1, shared by Pac-Man and the ghosts.
    int pacmanDeathFrame =
        -1; ///< Frame of the death animation, or -1 when not dying.

    bool mazeFlash = false;    ///< Draw the maze in white.
    bool showReady = false;    ///< Show the "READY!" text.
    bool showGameOver = false; ///< Show the "GAME OVER" text.
    bool paused = false;       ///< Freezes the animation.

    MazeTransform getMazeTransform(const Maze &maze) const;

    /// @brief Draws one sprite of the sheet, centered on a position.
    /// @param rect The part of the sheet to draw.
    /// @param position Where its center goes, in pixels.
    /// @param scale Scale of the sprite.
    void drawSprite(const sf::IntRect &rect, sf::Vector2f position,
                    float scale);

    void renderMaze(const Maze &maze, const MazeTransform &transform);
    void renderCollectibles(const Maze &maze, const MazeTransform &transform);
    void renderFruit(const FruitSpawner &fruitSpawner,
                     const MazeTransform &transform);
    void renderGhosts(const std::vector<std::unique_ptr<Ghost>> &ghosts,
                      const MazeTransform &transform);
    void renderPacman(const Pacman &pacman, const MazeTransform &transform);
    void renderPopups(const std::vector<ScorePopup> &popups,
                      const MazeTransform &transform);
    void renderBanner(const Maze &maze, const MazeTransform &transform,
                      const std::string &str, sf::Color color);
    void renderHudTop(const GameState &gameState,
                      const MazeTransform &transform);
    void renderHudBottom(const GameState &gameState, const Maze &maze,
                         const MazeTransform &transform);

  public:
    /// @param window The window to draw in.
    /// @throws std::runtime_error If an image or the font cannot be loaded.
    explicit Renderer(sf::RenderWindow &window);

    /// @brief Shows or hides the "READY!" text.
    void setShowReady(bool show);

    /// @brief Shows or hides the "GAME OVER" text.
    void setShowGameOver(bool show);

    /// @brief Freezes (or resumes) the animation of Pac-Man and the ghosts.
    void setPaused(bool value);

    /// @brief Draws the maze in white (true) or in its normal color (false),
    ///        for the flashing at the end of a round.
    void setMazeFlash(bool flash);

    /// @brief Makes Pac-Man be drawn with a frame of the death animation.
    /// @param frame From 0 to 11, or -1 to draw him normally.
    void setPacmanDeathFrame(int frame);

    /// @brief Draws a whole frame and shows it.
    /// @param pacman Pac-Man (not drawn while he is hidden).
    /// @param ghosts The ghosts.
    /// @param popups The scores being shown.
    /// @param gameState Score, lives and round, for the HUD.
    /// @param fruitSpawner Where the active fruit comes from.
    /// @param maze The maze and its pellets.
    void render(const Pacman &pacman,
                const std::vector<std::unique_ptr<Ghost>> &ghosts,
                const std::vector<ScorePopup> &popups,
                const GameState &gameState, const FruitSpawner &fruitSpawner,
                const Maze &maze);
};

#endif
