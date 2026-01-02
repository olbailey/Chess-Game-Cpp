//
// Created by olbai on 02/01/2026.
//

#ifndef BOARDSOUNDS_HPP
#define BOARDSOUNDS_HPP

#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <string>
#include <unordered_map>

class BoardSounds {
    static const std::array<std::string, 6> chessSoundNames;
    std::unordered_map<std::string, sf::SoundBuffer> chessSoundBuffers;
    std::vector<sf::Sound> activeSounds;
    std::vector<std::string> pendingSounds;
    int count = 0;

public:
    BoardSounds();

    void requestSound(const std::string& soundName);

    void update();

    void playSound(const std::string& soundName);

    void cleanup();
};



#endif //BOARDSOUNDS_HPP
