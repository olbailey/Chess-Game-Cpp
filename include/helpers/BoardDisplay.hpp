//
// Created by olbai on 04/07/2025.
//

#ifndef BOARDDISPLAY_H
#define BOARDDISPLAY_H

#include "Piece.hpp"
#include "../locators/Move.hpp"
#include "SFML/Graphics.hpp"

//Forward Declarations
class Board;

class BoardDisplay {
    static const sf::Color BLACK;
    static const sf::Color GREY;
    static const sf::Color PEACH;
    static const sf::Color BROWN;
    static const sf::Color YELLOW_HIGHLIGHT;
    static const sf::Color RED_HIGHLIGHT;
    static const sf::Color GREY_BACKGROUND;
    static const sf::Color WHITE_HIGHLIGHT;

    static const float MOVE_CIRCLE_RADIUS;
    static const float CHECK_CIRCLE_RADIUS;
    static const int PAWN_PROMOTION_RADIUS;

    const Board& board_;

    std::unique_ptr<sf::Font> fontPtr;

public:
    explicit BoardDisplay(const Board& board);

    void all() const;

    void printBoard() const;
    static void printBoard(const Board &board);

    void displayGameEndMessage() const;

private:
    void drawPattern() const;

    void drawCoordinates() const;

    void drawPieces() const;

    void drawPiece(const std::string &pieceID, const Move &move) const;

    void drawHighlightSquare(const Move &squareToHighlight, const sf::Color &colour) const;

    void drawHighlightCircle(const Move &squareToHighlight, const sf::Color &colour, float radius) const;

    void drawPossibleMoves(const Move &pieceSelected) const;

    void drawDraggedPiece() const;

    void drawMoveMadeHighlight() const;

    void drawUpgradingPawn() const;

    void drawClippedCircle(const Position &position, const sf::Color &squareColour,
                           const sf::Color &circleColour) const;

    void setFont();
};

#endif //BOARDDISPLAY_H
