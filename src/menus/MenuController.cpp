#include "menus/MenuController.hpp"
#include "game/Leaderboard.hpp"

MenuController::MenuController(Menu &menu, Leaderboard &leaderboard)
    : menu{menu}, leaderboard{leaderboard} {}

Menu::Action MenuController::handleEvent(const sf::Event &event) {
    switch (menu.getScreen()) {
    case Menu::Screen::Main:
        return handleMain(event);

    case Menu::Screen::Leaderboard:
        handleLeaderboard(event);
        break;

    case Menu::Screen::NameEntry:
        handleNameEntry(event);
        break;
    }

    return Menu::Action::None;
}

Menu::Action MenuController::handleMain(const sf::Event &event) {
    const auto *key = event.getIf<sf::Event::KeyPressed>();
    if (!key)
        return Menu::Action::None;

    switch (key->code) {
    case sf::Keyboard::Key::Up:
    case sf::Keyboard::Key::W:
        menu.moveUp();
        break;

    case sf::Keyboard::Key::Down:
    case sf::Keyboard::Key::S:
        menu.moveDown();
        break;

    case sf::Keyboard::Key::Enter:
    case sf::Keyboard::Key::Space:
        return menu.select();

    default:
        break;
    }

    return Menu::Action::None;
}

void MenuController::handleLeaderboard(const sf::Event &event) {
    const auto *key = event.getIf<sf::Event::KeyPressed>();
    if (!key)
        return;

    if (key->code == sf::Keyboard::Key::Escape ||
        key->code == sf::Keyboard::Key::Enter ||
        key->code == sf::Keyboard::Key::Backspace)
        menu.back();
}

void MenuController::handleNameEntry(const sf::Event &event) {
    if (const auto *text = event.getIf<sf::Event::TextEntered>()) {
        if (text->unicode < 128)
            menu.typeLetter(static_cast<char>(text->unicode));
    }

    if (const auto *key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Backspace) {
            menu.eraseLetter();
        } else if (key->code == sf::Keyboard::Key::Enter &&
                   menu.isNameComplete()) {
            leaderboard.add(menu.getName(), menu.getPendingScore());
            menu.showMain();
        }
    }
}

void MenuController::onGameOver(int score) {
    if (leaderboard.qualifies(score))
        menu.startNameEntry(score);
    else
        menu.showMain();
}
