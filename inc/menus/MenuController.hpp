#ifndef MENUCONTROLLER_HPP
#define MENUCONTROLLER_HPP

#include "menus/Menu.hpp"

#include <SFML/Window/Event.hpp>

class Leaderboard;

/// @brief Turns the player's input into commands for the menu.
///
/// It is the only place that knows which keys do what in the menus. It also
/// connects the menu to the leaderboard: it saves a new record when the name
/// is confirmed and decides what comes after a game.
class MenuController {
  private:
    Menu &menu;               ///< The menu it commands.
    Leaderboard &leaderboard; ///< Where new records are saved.

    Menu::Action handleMain(const sf::Event &event);
    void handleLeaderboard(const sf::Event &event);
    void handleNameEntry(const sf::Event &event);

  public:
    /// @param menu The menu to command.
    /// @param leaderboard The leaderboard that receives new records.
    MenuController(Menu &menu, Leaderboard &leaderboard);

    /// @brief Handles one window event while a menu is shown.
    ///
    /// Main menu: up/down (or W/S) move, Enter/Space choose. Leaderboard:
    /// Escape, Enter or Space go back. Name entry: letters type, Backspace
    /// erases and Enter confirms (only with all the letters).
    /// @param event The event from the window.
    /// @return What Game has to do (start the game or quit), or
    ///         Menu::Action::None.
    Menu::Action handleEvent(const sf::Event &event);

    /// @brief Shows what comes after a game: the name entry if the score
    ///        earned a place on the leaderboard, otherwise the main menu.
    /// @param score The final score of the game.
    void onGameOver(int score);
};

#endif
