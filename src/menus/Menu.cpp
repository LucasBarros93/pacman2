#include "menus/Menu.hpp"
#include <cctype>

void Menu::moveUp() {
    selected = (selected + OPTIONS.size() - 1) % OPTIONS.size();
}

void Menu::moveDown() { selected = (selected + 1) % OPTIONS.size(); }

Menu::Action Menu::select() {
    switch (static_cast<Option>(selected)) {
    case Option::Play:
        return Action::Play;

    case Option::Leaderboard:
        screen = Screen::Leaderboard;
        return Action::None;

    case Option::Quit:
        return Action::Quit;
    }

    return Action::None;
}

void Menu::back() { showMain(); }

void Menu::typeLetter(char letter) {
    const auto code = static_cast<unsigned char>(letter);

    if (!std::isalpha(code) || nameBuffer.size() >= NAME_LENGTH)
        return;

    nameBuffer += static_cast<char>(std::toupper(code));
}

void Menu::eraseLetter() {
    if (!nameBuffer.empty())
        nameBuffer.pop_back();
}

void Menu::showMain() {
    screen = Screen::Main;
    selected = 0;
}

void Menu::startNameEntry(int score) {
    pendingScore = score;
    nameBuffer.clear();
    screen = Screen::NameEntry;
}

bool Menu::isNameComplete() const { return nameBuffer.size() == NAME_LENGTH; }

Menu::Screen Menu::getScreen() const { return screen; }

std::size_t Menu::getSelected() const { return selected; }

const std::string &Menu::getName() const { return nameBuffer; }

int Menu::getPendingScore() const { return pendingScore; }
