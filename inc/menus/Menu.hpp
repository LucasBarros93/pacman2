#ifndef MENU_HPP
#define MENU_HPP

#include <array>
#include <cstddef>
#include <string>

class Menu {
  public:
    enum class Screen { Main, Leaderboard, NameEntry };
    enum class Action { None, Play, Quit };
    enum class Option { Play, Leaderboard, Quit };

    static constexpr std::size_t NAME_LENGTH = 3;
    static constexpr std::array<const char *, 3> OPTIONS{"PLAY", "LEADERBOARD",
                                                         "QUIT"};

  private:
    Screen screen = Screen::Main;
    std::size_t selected = 0;
    std::string nameBuffer;
    int pendingScore = 0;

  public:
    void moveUp();
    void moveDown();
    Action select();
    void back();
    void typeLetter(char letter);
    void eraseLetter();
    void showMain();
    void startNameEntry(int score);

    Screen getScreen() const;
    std::size_t getSelected() const;
    const std::string &getName() const;
    int getPendingScore() const;
    bool isNameComplete() const;
};

#endif
