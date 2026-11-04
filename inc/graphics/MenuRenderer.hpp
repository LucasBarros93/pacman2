#ifndef MENURENDERER_HPP
#define MENURENDERER_HPP

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "game/Leaderboard.hpp"
#include "menus/Menu.hpp"
#include <string>

class MenuRenderer {
  private:
    sf::RenderWindow &window;
    sf::Font font;

    void drawCentered(const std::string &str, unsigned int size,
                      sf::Color color, float y);

    void renderMain(const Menu &menu);
    void renderLeaderboard(const Leaderboard &leaderboard);
    void renderNameEntry(const Menu &menu);

  public:
    explicit MenuRenderer(sf::RenderWindow &window);

    void render(const Menu &menu, const Leaderboard &leaderboard);
};

#endif
