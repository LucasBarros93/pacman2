#ifndef MENUCONTROLLER_HPP
#define MENUCONTROLLER_HPP

#include "menus/Menu.hpp"

#include <SFML/Window/Event.hpp>

class Leaderboard;

class MenuController {
  private:
    Menu &menu;
    Leaderboard &leaderboard;

    Menu::Action handleMain(const sf::Event &event);
    void handleLeaderboard(const sf::Event &event);
    void handleNameEntry(const sf::Event &event);

  public:
    MenuController(Menu &menu, Leaderboard &leaderboard);

    Menu::Action handleEvent(const sf::Event &event);

    void onGameOver(int score);
};

#endif
