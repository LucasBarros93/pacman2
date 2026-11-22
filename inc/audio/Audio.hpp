#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <array>

/// @brief Loads and plays all the sounds of the game.
///
/// Most sounds are played once (intro, chomp, fruit, ghost eaten, death). Two
/// of them are loops that follow what the ghosts are doing: the frightened
/// sound and the one of the eyes going back to the house.
class Audio {
  private:
    // the buffers must come before the sounds that use them
    sf::SoundBuffer introBuffer;
    sf::SoundBuffer chompBuffer;
    sf::SoundBuffer eatFruitBuffer;
    sf::SoundBuffer eatGhostBuffer;
    sf::SoundBuffer deathBuffer;
    sf::SoundBuffer ghostBuffer;
    sf::SoundBuffer frightenedBuffer;
    sf::SoundBuffer deadBuffer;

    sf::Sound intro;
    sf::Sound chomp;
    sf::Sound eatFruit;
    sf::Sound eatGhost;
    sf::Sound death;
    sf::Sound ghostLoop;
    sf::Sound frightenedLoop;
    sf::Sound deadLoop;

    /// @brief Every sound, to pause, resume or stop them all at once.
    std::array<sf::Sound *, 8> allSounds();

    /// @brief Starts a loop if it is off and should be on, or stops it if it
    ///        is on and should be off.
    void setLoop(sf::Sound &sound, bool on);

  public:
    /// @throws std::runtime_error If a sound file cannot be loaded.
    Audio();

    void playIntro();

    /// @brief Plays the chomp, unless it is already playing (so a sequence of
    ///        pellets sounds continuous).
    void playChomp();

    void playEatFruit();
    void playEatGhost();
    void playDeath();

    /// @brief Turns the ghost loops on or off. The eyes have priority: while
    ///        any of them is on the way home, the frightened sound is silent.
    /// @param frightened Whether any ghost is frightened.
    /// @param dead Whether any ghost is only a pair of eyes.
    void setGhostLoops(bool frightened, bool dead);

    /// @brief How long the intro lasts, in seconds.
    [[nodiscard]] float getIntroDuration() const;

    void pause();  ///< Pauses everything that is playing.
    void resume(); ///< Resumes what pause() paused.
    void stopAll();
};

#endif
