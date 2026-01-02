//
// Created by olbai on 30/06/2025.
//

#include "SFML/Graphics.hpp"

#include "Board.hpp"
#include "helpers/TextureManager.hpp"

Board::Board(sf::RenderWindow& window, const TextureManager& textureManager, BoardSounds& boardSounds, const float squareSize)
    : boardDisplay(std::make_unique<BoardDisplay>(*this)),
      fenProcessor(std::make_unique<FenProcessor>(*this)),
      chessLogic(std::make_unique<ChessLogic>(*this)),
      aiCalculator(std::make_unique<AICalculator>(aiDepth, aiColour)),
      window(window), textureManager(textureManager), boardSounds(boardSounds), squareSize(squareSize) {
}

Board::Board(const Board &other)
    : chessLogic(std::make_unique<ChessLogic>(*this)),
      window(other.window), textureManager(other.textureManager), boardSounds(other.boardSounds), squareSize(other.squareSize) {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (const Piece *piecePtr = other.getPiece(i, j); piecePtr != nullptr) {
                grid[i][j] = Piece::setPiece(Piece::pieceToLetter(*piecePtr), piecePtr->getPiecePosition());
                grid[i][j]->setMoves(piecePtr->getMoves());
    }

    if (const Move *movePtr = other.enpassantLocation.get(); movePtr != nullptr)
        enpassantLocation = std::make_unique<Move>(*movePtr);
    if (const Move *movePtr = other.pawnUpgradedPosition.get(); movePtr != nullptr)
        pawnUpgradedPosition = std::make_unique<Move>(*movePtr);

    playerTurn = other.playerTurn;
}

Board::~Board() {
    if (aiThread.joinable())
        aiThread.join();
}

void Board::handleEvents() {
    sf::Event event{};

    while (window.pollEvent(event)) {
        using sf::Event;
        using sf::Mouse;
        switch (event.type) {
            case Event::EventType::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Button::Left) {
                    leftMousePressedLogic();
                }
                break;
            case Event::EventType::MouseButtonReleased:
                if (event.mouseButton.button == Mouse::Button::Left) {
                    leftMouseReleasedLogic();
                }
                break;
            case Event::Closed:
                window.close();
            default:;
        }
    }
}

void Board::gameLoop() {
    if (winnerValue != Winner::notFinished) {
        boardDisplay->displayGameEndMessage();

    } else if (moveMade) {
        swapPlayerTurn();
        chessLogic->calculateMoves(false);
        moveMade = false;
        if (checkLocation != nullptr)
            boardSounds.requestSound("check");


        aiManagement();
    } else {
        isGameFinished();
    }

    handleEvents();
    checkIfDragged();
    boardSounds.update();
}

void Board::startAiThread() {
    const AIMove &aiMove = aiCalculator->pickMove(*this);
    makeMove(getPiece(aiMove.piecePosition_), aiMove.moveToMake_);
    moveMade = true;
}

void Board::aiManagement() {
    if (!aiActivated) return;

    if (isAiThinking) {
        isAiThinking = false;

        if (aiThread.joinable())
            aiThread.join();

    } else if (aiColour == playerTurn) {
        isAiThinking = true;
        aiThread = std::thread(&Board::startAiThread, this);
    }
}