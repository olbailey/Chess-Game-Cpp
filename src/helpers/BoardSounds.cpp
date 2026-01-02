//
// Created by olbai on 02/01/2026.
//

#include "helpers/BoardSounds.hpp"

#include <iostream>

const std::array<std::string, 6> BoardSounds::chessSoundNames = {
    "capture", "castle", "check",
    "game-end", "move", "promote"
};

BoardSounds::BoardSounds() {
    for (const std::string& chessSoundName : chessSoundNames) {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile("../resources/sounds/" + chessSoundName + ".wav")) {
            std::cerr << "Failed to load Sound: " << chessSoundName << std::endl;
            continue;
        }

        chessSoundBuffers[chessSoundName] = buffer;
    }
}

void BoardSounds::requestSound(const std::string &soundName) {
    pendingSounds.push_back(soundName);
}

void BoardSounds::update() {
    cleanup();

    if (!pendingSounds.empty()) {
        if (count >= 5) {
            playSound(pendingSounds.back());
            pendingSounds.clear();
            count = 0;
        } else {
            count++;
        }
    }
}

void BoardSounds::playSound(const std::string& soundName) {
    const auto it = chessSoundBuffers.find(soundName);
    if (it == chessSoundBuffers.end()) {
        std::cerr << "Sound not found: " << soundName << "\n";
        return;
    }

    activeSounds.emplace_back();
    sf::Sound& sound = activeSounds.back();
    sound.setBuffer(it->second);
    sound.play();

}

void BoardSounds::cleanup() {
    activeSounds.erase(
        std::ranges::remove_if(activeSounds,
                               [](const sf::Sound& s) {
                                   return s.getStatus() == sf::Sound::Stopped;
                               }).begin(),
        activeSounds.end()
    );
}

