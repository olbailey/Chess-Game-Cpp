//
// Created by olbai on 04/07/2025.
//

#include "SFML/Graphics.hpp"

#include "helpers/BoardDisplay.hpp"
#include "Board.hpp"
#include "Piece.hpp"

const sf::Color BoardDisplay::BLACK = sf::Color(0, 0, 0);
const sf::Color BoardDisplay::GREY = sf::Color(20, 20, 20, 50);
const sf::Color BoardDisplay::PEACH = sf::Color(240, 217, 181);
const sf::Color BoardDisplay::BROWN = sf::Color(181, 136, 99);
const sf::Color BoardDisplay::YELLOW_HIGHLIGHT = sf::Color(251, 236, 93, 100);
const sf::Color BoardDisplay::RED_HIGHLIGHT = sf::Color(200, 0, 0, 75);
const sf::Color BoardDisplay::GREY_BACKGROUND = sf::Color(20, 20, 20, 150);
const sf::Color BoardDisplay::WHITE_HIGHLIGHT = sf::Color(255, 255, 255, 150);

const float BoardDisplay::MOVE_CIRCLE_RADIUS = 20;
const float BoardDisplay::CHECK_CIRCLE_RADIUS = 35;
const int BoardDisplay::PAWN_PROMOTION_RADIUS = 58;


BoardDisplay::BoardDisplay(const Board &board)
    : board_(board) {
    setFont();
}


void BoardDisplay::all() const {
    drawPattern();
    drawCoordinates();

    if (board_.checkLocation != nullptr)
        drawHighlightCircle(*board_.checkLocation, RED_HIGHLIGHT, CHECK_CIRCLE_RADIUS);

    if (board_.pieceBeingDragged && board_.coordClicked != nullptr) { // if piece being dragged
        drawHighlightSquare(*board_.coordClicked, YELLOW_HIGHLIGHT);
        drawPossibleMoves(*board_.coordClicked);

    } else if (board_.pieceSelected != nullptr) { // if piece selected
        drawHighlightSquare(*board_.pieceSelected, YELLOW_HIGHLIGHT);
        drawPossibleMoves(*board_.pieceSelected);
    }

    drawMoveMadeHighlight();
    drawPieces();

    if (board_.pawnUpgradedPosition != nullptr) { // if piece being upgraded
        drawUpgradingPawn();
    }

    if (board_.pieceBeingDragged && board_.pawnUpgradedPosition == nullptr) {
        drawDraggedPiece();
    }
}

void BoardDisplay::drawPattern() const {
    sf::RectangleShape square(sf::Vector2f(board_.squareSize, board_.squareSize));
    bool isPeach = true;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (isPeach)
                square.setFillColor(PEACH);
            else
                square.setFillColor(BROWN);

            square.setPosition(board_.squareSize * i, board_.squareSize * j);
            board_.window.draw(square);

            isPeach = !isPeach;
        }

        isPeach = !isPeach;
    }
}

void BoardDisplay::drawCoordinates() const {
    sf::Text text;

    text.setFont(*fontPtr);
    text.setCharacterSize(16);

    for (int i = 0; i < 8; ++i) {
        constexpr std::array letters{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
        const float letterOffsetX = board_.squareSize - 12;
        const float letterOffsetY = board_.squareSize - 20;
        constexpr float numberOffset = 5;

        text.setFillColor(i % 2 == 0 ? PEACH : BROWN);

        text.setString(letters[i]);
        text.setPosition(board_.squareSize * i + letterOffsetX, board_.squareSize * 7 + letterOffsetY);
        board_.window.draw(text);

        text.setString(std::to_string(i + 1));
        text.setPosition(numberOffset, board_.squareSize * (7 - i) + numberOffset);
        board_.window.draw(text);
    }
}


void BoardDisplay::printBoard() const {
    printBoard(board_);
}

void BoardDisplay::printBoard(const Board &board) {
    int count = 0;
    for (const Piece* piecePtr : board) {
        if (piecePtr != nullptr) {
            const char pieceLetter = Piece::pieceToLetter(*piecePtr);

            std::cout << pieceLetter << " ";
        } else
            std::cout << "-" << " ";

        count ++;

        if (count == 8) {
            count = 0;
        std::cout << '\n';
        }
    }
    std::cout << '\n';
}

void BoardDisplay::drawPieces() const {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (const Piece* piecePtr = board_.getPiece(i, j); piecePtr != nullptr) {
                if (board_.pawnUpgradedPosition != nullptr // Doesn't draw pawn being upgraded
                    && piecePtr->getPiecePosition().coordEqualTo(*board_.pawnUpgradedPosition.get())) continue;

                drawPiece(piecePtr->getPieceId(), Move({i, j}));
    }
}

void BoardDisplay::drawPiece(const std::string &pieceID, const Move &move) const {
    sf::Sprite sprite(board_.textureManager.getTexture(pieceID));
    const auto& spriteRect = sprite.getTextureRect().getSize();
    const float xOffset = (board_.squareSize - spriteRect.x) / 2;
    const float yOffset = (board_.squareSize - spriteRect.y) / 2;

    sprite.setPosition(board_.squareSize * move.y + xOffset, board_.squareSize * move.x + yOffset);
    board_.window.draw(sprite);
}

void BoardDisplay::drawHighlightSquare(const Move& squareToHighlight, const sf::Color &colour) const {
    sf::RectangleShape highlight(sf::Vector2f(board_.squareSize, board_.squareSize));
    highlight.setFillColor(colour);

    highlight.setPosition(squareToHighlight.y * board_.squareSize, squareToHighlight.x * board_.squareSize);

    board_.window.draw(highlight);
}

void BoardDisplay::drawHighlightCircle(const Move& squareToHighlight, const sf::Color &colour, const float radius) const {
    sf::CircleShape highlight(radius);
    highlight.setFillColor(colour);

    const float centeringFactor = board_.squareSize / 2 - radius;
    highlight.setPosition(squareToHighlight.y * board_.squareSize + centeringFactor,
        squareToHighlight.x * board_.squareSize + centeringFactor);

    board_.window.draw(highlight);
}

void BoardDisplay::drawPossibleMoves(const Move &pieceSelected) const {
    for (const Piece* piece = board_.getPiece(pieceSelected);
            const Move &move : piece->getPossibleMoves(Piece::DRAWABLE_MOVETYPES)) {

        if (const Move currentMouseCoord = board_.getBoardCoord(board_.getMousePos());
                currentMouseCoord.coordEqualTo(move))
            drawHighlightSquare(currentMouseCoord, YELLOW_HIGHLIGHT);

        drawHighlightCircle(move, GREY, MOVE_CIRCLE_RADIUS);
    }
}


void BoardDisplay::drawDraggedPiece() const {
    if (board_.coordClicked == nullptr) throw std::runtime_error("trying to draw dragged piece that does not exist"); //TODO temp for testing

    const Position mousePos = board_.getMousePos();
    const Piece* piece = board_.getPiece(*board_.coordClicked);
    sf::Sprite sprite(board_.textureManager.getTexture(piece->getPieceId()));
    const auto& spriteRect = sprite.getTextureRect().getSize();
    const float xOffset = board_.squareSize - spriteRect.x;
    const float yOffset = board_.squareSize - spriteRect.y; // TODO piece slightly off centre

    sprite.setPosition(mousePos.x - xOffset, mousePos.y - yOffset);
    board_.window.draw(sprite);
}

void BoardDisplay::drawMoveMadeHighlight() const {
    if (board_.moveJustMade == nullptr) return;
    if (board_.pawnUpgradedPosition != nullptr) return;

    drawHighlightSquare(board_.moveJustMade->at(0), YELLOW_HIGHLIGHT);
    drawHighlightSquare(board_.moveJustMade->at(1), YELLOW_HIGHLIGHT);
}

void BoardDisplay::drawUpgradingPawn() const {
    const Move &pawnCoord = *board_.pawnUpgradedPosition;
    const PieceColour &pawnColour = board_.getPiece(pawnCoord)->getPieceColour();
    const int directionMultiplier = (pawnColour == PieceColour::White) ? 1 : -1;

    for (int i = 0; i < 4; ++i) {
        const Move squareCoord({pawnCoord.x + i * directionMultiplier, pawnCoord.y});
        if (const Move currentMouseCoord = board_.getBoardCoord(board_.getMousePos());
                currentMouseCoord.coordEqualTo(squareCoord))
            drawClippedCircle(squareCoord.toPosition(board_.squareSize), GREY_BACKGROUND,
                              WHITE_HIGHLIGHT);
        else {
            drawHighlightSquare(squareCoord, WHITE_HIGHLIGHT);
        }

        drawPiece(Piece::getPawnPromotionId(i, pawnColour), squareCoord);
    }
}

void BoardDisplay::drawClippedCircle(const Position &position, const sf::Color &squareColour,
                                     const sf::Color &circleColour) const {
    const float centeringFactor = board_.squareSize / 2 - PAWN_PROMOTION_RADIUS;
    // Defining the square
    sf::CircleShape outerCircle(PAWN_PROMOTION_RADIUS);
    outerCircle.setPosition(centeringFactor, centeringFactor); // This circle will extend beyond the rectangle
    outerCircle.setFillColor(sf::Color::Transparent);
    outerCircle.setOutlineThickness(20);
    outerCircle.setOutlineColor(squareColour);

    // Defining the circle
    sf::CircleShape middleCircle(PAWN_PROMOTION_RADIUS);
    middleCircle.setPosition(centeringFactor, centeringFactor);
    middleCircle.setFillColor(circleColour);

    // Creating a RenderTexture the size of the clipping rectangle
    sf::RenderTexture renderTexture;
    renderTexture.create(board_.squareSize, board_.squareSize);

    // Creating a Sprite to draw the content of the RenderTexture
    sf::Sprite renderSprite(renderTexture.getTexture());
    renderSprite.setPosition(sf::Vector2f(position.x, position.y));
    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(outerCircle);
    renderTexture.draw(middleCircle);

    board_.window.draw(renderSprite);
}

void BoardDisplay::setFont() {
    if (sf::Font font; font.loadFromFile("../resources/fonts/arial_narrow_7.ttf")) // Make sure "arial.ttf" is in the same directory as your executable
    {
        fontPtr = std::make_unique<sf::Font>(font);
    } else {
        std::cerr << "Error loading font file! Make sure 'arial.ttf' is present." << std::endl;
    }
}

void BoardDisplay::displayGameEndMessage() const {
    sf::Text text;

    text.setFont(*fontPtr);
    text.setCharacterSize(60);

    text.setFillColor(BLACK);

    switch (board_.winnerValue) {
        case Winner::White :
            text.setString("White Won!");
            break;
        case Winner::Black :
            text.setString("Black Won!");
            break;
        case Winner::Draw :
            text.setString("Draw!");
        default: ;
    }

    const sf::FloatRect bounds = text.getLocalBounds();
    text.setPosition(board_.squareSize * 4 - bounds.width / 2, board_.squareSize * 4 - bounds.height);
    board_.window.draw(text);
}
