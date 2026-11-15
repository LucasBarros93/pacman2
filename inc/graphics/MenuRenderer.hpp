#ifndef MENURENDERER_HPP
#define MENURENDERER_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

class Leaderboard;
class Menu;

/// @brief Draws the menu screens (main menu, leaderboard and name entry).
///
/// It only reads the Menu and the Leaderboard; it never changes them.
class MenuRenderer {
  private:
    sf::RenderWindow &window;
    sf::Font font; ///< Same font as the HUD.

    /// @brief Draws a text centered horizontally.
    /// @param str The text.
    /// @param size Character size, in pixels.
    /// @param color Text color.
    /// @param y Top of the text, in pixels.
    void drawCentered(const std::string &str, unsigned int size,
                      sf::Color color, float y);

    void renderMain(const Menu &menu);
    void renderLeaderboard(const Leaderboard &leaderboard);
    void renderNameEntry(const Menu &menu);

  public:
    /// @param window The window to draw in.
    /// @throws std::runtime_error If the font cannot be loaded.
    explicit MenuRenderer(sf::RenderWindow &window);

    /// @brief Draws the screen the menu is showing and displays it.
    /// @param menu Says which screen to draw.
    /// @param leaderboard The scores for the leaderboard screen.
    void render(const Menu &menu, const Leaderboard &leaderboard);
};

#endif
