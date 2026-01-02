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

    Board board(window, textureManager, SQUARE_LENGTH);
    board.setPosition("r1b1k2r/1ppp1pQp/p4p2/2b1pN2/2B1Pn2/3P3P/PPq2PP1/RN3RK1 b - - 0 1"); //FenProcessor::STARTING_FEN

    while (window.isOpen()) {
        window.clear(sf::Color::Black);
        board.draw();
        board.gameLoop();
        window.display();
    }

    return 0;
}