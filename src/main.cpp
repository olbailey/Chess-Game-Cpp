#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Board.hpp"
#include "helpers/TextureManager.hpp"

#include "ai_classes/AICalculator.hpp"

int main()
{
    constexpr float SQUARE_LENGTH = 100;
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!", sf::Style::Close);

    const TextureManager textureManager;
    BoardSounds boardSounds;

    Board board(window, textureManager, boardSounds, SQUARE_LENGTH);
    board.setPosition(FenProcessor::STARTING_FEN);

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        board.draw();
        board.gameLoop();
        window.display();
    }

    return 0;
}