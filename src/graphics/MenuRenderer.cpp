#include "graphics/MenuRenderer.hpp"

#include <SFML/Graphics/Text.hpp>

#include <stdexcept>
#include <string>

MenuRenderer::MenuRenderer(sf::RenderWindow &window) : window{window} {
    if (!font.openFromFile("assets/fonts/pacman.ttf"))
        throw std::runtime_error("Could not load menu font");
}

void MenuRenderer::drawCentered(const std::string &str, unsigned int size,
                                sf::Color color, float y) {
    sf::Text text{font, str, size};
    text.setFillColor(color);

    const sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(
        {window.getSize().x / 2.f - (bounds.position.x + bounds.size.x / 2.f),
         y});

    window.draw(text);
}

void MenuRenderer::renderMain(const Menu &menu) {
    drawCentered("PACMAN", 56, sf::Color::Yellow, 90.f);

    for (std::size_t i = 0; i < Menu::OPTIONS.size(); ++i) {
        const sf::Color color =
            (i == menu.getSelected()) ? sf::Color::Blue : sf::Color::White;

        drawCentered(Menu::OPTIONS[i], 26, color,
                     270.f + static_cast<float>(i) * 60.f);
    }
}

void MenuRenderer::renderLeaderboard(const Leaderboard &leaderboard) {
    drawCentered("LEADERBOARD", 36, sf::Color::Yellow, 70.f);

    const float centerX = window.getSize().x / 2.f;
    const auto &entries = leaderboard.getEntries();

    for (std::size_t i = 0; i < Leaderboard::MAX_ENTRIES; ++i) {
        const float y = 180.f + static_cast<float>(i) * 55.f;
        const bool filled = i < entries.size();

        const std::string name = filled ? entries[i].name : "---";
        const std::string score =
            filled ? std::to_string(entries[i].score) : "---";

        sf::Text left{font, std::to_string(i + 1) + ".  " + name, 24};
        left.setPosition({centerX - 200.f, y});

        sf::Text right{font, score, 24};
        const sf::FloatRect bounds = right.getLocalBounds();
        right.setPosition(
            {centerX + 200.f - (bounds.position.x + bounds.size.x), y});

        window.draw(left);
        window.draw(right);
    }

    drawCentered("PRESS ENTER TO GO BACK", 14, sf::Color(150, 150, 150), 520.f);
}

void MenuRenderer::renderNameEntry(const Menu &menu) {
    drawCentered("NEW HIGH SCORE", 34, sf::Color::Yellow, 100.f);
    drawCentered(std::to_string(menu.getPendingScore()), 30, sf::Color::White,
                 170.f);
    drawCentered("ENTER YOUR NAME", 20, sf::Color::White, 270.f);

    const std::string &name = menu.getName();
    std::string slots;

    for (std::size_t i = 0; i < Menu::NAME_LENGTH; ++i) {
        slots += (i < name.size()) ? name[i] : '_';
        if (i + 1 < Menu::NAME_LENGTH)
            slots += ' ';
    }

    drawCentered(slots, 48, sf::Color::Yellow, 330.f);
    drawCentered("PRESS ENTER TO CONFIRM", 14, sf::Color(150, 150, 150), 480.f);
}

void MenuRenderer::render(const Menu &menu, const Leaderboard &leaderboard) {
    window.setView(window.getDefaultView());
    window.clear();

    switch (menu.getScreen()) {
    case Menu::Screen::Main:
        renderMain(menu);
        break;

    case Menu::Screen::Leaderboard:
        renderLeaderboard(leaderboard);
        break;

    case Menu::Screen::NameEntry:
        renderNameEntry(menu);
        break;
    }

    window.display();
}
