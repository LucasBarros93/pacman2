#include "audio/Audio.hpp"

#include <stdexcept>
#include <string>

namespace {
using Status = sf::SoundSource::Status;

constexpr const char *INTRO_PATH = "assets/audios/intro.mp3";
constexpr const char *CHOMP_PATH = "assets/audios/chomping.mp3";
constexpr const char *EAT_FRUIT_PATH = "assets/audios/eating-fruit.mp3";
constexpr const char *EAT_GHOST_PATH = "assets/audios/eating-ghost.mp3";
constexpr const char *DEATH_PATH = "assets/audios/death.mp3";
constexpr const char *GHOST_PATH = "assets/audios/ghost.mp3";
constexpr const char *FRIGHTENED_PATH = "assets/audios/ghost-frighted.mp3";
constexpr const char *DEAD_PATH = "assets/audios/ghost-dead.mp3";
constexpr const char *UP_PATH = "assets/audios/up.mp3";

sf::SoundBuffer loadBuffer(const std::string &path) {
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(path))
        throw std::runtime_error("Could not load sound: " + path);

    return buffer;
}
} // namespace

Audio::Audio()
    : introBuffer(loadBuffer(INTRO_PATH)), chompBuffer(loadBuffer(CHOMP_PATH)),
      eatFruitBuffer(loadBuffer(EAT_FRUIT_PATH)),
      eatGhostBuffer(loadBuffer(EAT_GHOST_PATH)),
      deathBuffer(loadBuffer(DEATH_PATH)), ghostBuffer(loadBuffer(GHOST_PATH)),
      frightenedBuffer(loadBuffer(FRIGHTENED_PATH)),
      deadBuffer(loadBuffer(DEAD_PATH)), upBuffer(loadBuffer(UP_PATH)),
      intro(introBuffer), chomp(chompBuffer), eatFruit(eatFruitBuffer),
      eatGhost(eatGhostBuffer), death(deathBuffer), ghostLoop(ghostBuffer),
      frightenedLoop(frightenedBuffer), deadLoop(deadBuffer), up(upBuffer) {
    frightenedLoop.setLooping(true);
    deadLoop.setLooping(true);
}

std::array<sf::Sound *, 9> Audio::allSounds() {
    return {&intro,     &chomp,          &eatFruit, &eatGhost, &death,
            &ghostLoop, &frightenedLoop, &deadLoop, &up};
}

void Audio::setLoop(sf::Sound &sound, bool on) {
    const Status status = sound.getStatus();

    if (on && status == Status::Stopped)
        sound.play();
    else if (!on && status != Status::Stopped)
        sound.stop();
}

void Audio::playIntro() { intro.play(); }

void Audio::playChomp() {
    if (chomp.getStatus() != Status::Playing)
        chomp.play();
}

void Audio::playEatFruit() { eatFruit.play(); }

void Audio::playEatGhost() { eatGhost.play(); }

void Audio::playDeath() { death.play(); }

void Audio::playUp() { up.play(); }

void Audio::setGhostLoops(bool frightened, bool dead) {
    setLoop(ghostLoop, !dead && !frightened);
    setLoop(deadLoop, dead);
    setLoop(frightenedLoop, frightened && !dead);
}

float Audio::getIntroDuration() const {
    return introBuffer.getDuration().asSeconds();
}

void Audio::pause() {
    for (sf::Sound *sound : allSounds())
        if (sound->getStatus() == Status::Playing)
            sound->pause();
}

void Audio::resume() {
    for (sf::Sound *sound : allSounds())
        if (sound->getStatus() == Status::Paused)
            sound->play();
}

void Audio::stopAll() {
    for (sf::Sound *sound : allSounds())
        sound->stop();
}
