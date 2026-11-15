#ifndef MENU_HPP
#define MENU_HPP

#include <array>
#include <cstddef>
#include <string>

/// @brief The state of the menus: which screen is shown, which option is
///        highlighted and the name being typed.
///
/// It knows nothing about keys or drawing: MenuController tells it what the
/// player did and MenuRenderer draws what it says.
class Menu {
  public:
    /// @brief The screens of the menu.
    enum class Screen {
        Main,        ///< Play, leaderboard and quit.
        Leaderboard, ///< The best scores.
        NameEntry    ///< Typing the name for a new record.
    };

    /// @brief What Game has to do after the player chose something.
    enum class Action {
        None, ///< Nothing.
        Play, ///< Start a new game.
        Quit  ///< Close the program.
    };

    static constexpr std::size_t NAME_LENGTH = 3; ///< Letters in a name.

    /// @brief Labels of the main menu, in the order they are shown.
    static constexpr std::array<const char *, 3> OPTIONS{"PLAY", "LEADERBOARD",
                                                         "QUIT"};

  private:
    /// Entries of the main menu, in the same order as OPTIONS.
    enum class Option { Play, Leaderboard, Quit };

    Screen screen = Screen::Main; ///< Screen being shown.
    std::size_t selected =
        0; ///< Highlighted entry of the main menu (index in OPTIONS).
    std::string nameBuffer; ///< Letters typed so far in the name entry.
    int pendingScore = 0;   ///< Score the name entry is for.

  public:
    /// @brief Moves the highlight up in the main menu, wrapping around.
    void moveUp();

    /// @brief Moves the highlight down in the main menu, wrapping around.
    void moveDown();

    /// @brief Chooses the highlighted option of the main menu.
    /// @return Action::Play or Action::Quit when the game has to react.
    ///         Choosing the leaderboard only changes the screen, so it
    ///         returns Action::None.
    Action select();

    /// @brief Shows the main menu, with the first option highlighted.
    void showMain();

    /// @brief Starts the name entry for a new record.
    /// @param score The score that earned the place.
    void startNameEntry(int score);

    /// @brief Adds a letter to the name, in upper case.
    ///
    /// Anything that is not a letter is ignored, and so is any letter once
    /// the name has NAME_LENGTH letters.
    /// @param letter The character typed.
    void typeLetter(char letter);

    /// @brief Removes the last letter of the name, if there is one.
    void eraseLetter();

    [[nodiscard]] Screen getScreen() const;

    /// @brief Index in OPTIONS of the highlighted entry.
    [[nodiscard]] std::size_t getSelected() const;

    /// @brief The letters typed so far.
    [[nodiscard]] const std::string &getName() const;

    /// @brief The score the name entry is for.
    [[nodiscard]] int getPendingScore() const;

    /// @brief Whether the name has all its NAME_LENGTH letters.
    [[nodiscard]] bool isNameComplete() const;
};

#endif
