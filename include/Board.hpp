//
// Created by olbai on 30/06/2025.
//

#ifndef BOARD_H
#define BOARD_H

#include "SFML/Graphics.hpp"

#include "Piece.hpp"
#include "helpers/TextureManager.hpp"
#include "helpers/BoardDisplay.hpp"
#include "helpers/FenProcessor.hpp"
#include "ChessLogic.hpp"
#include "helpers/BoardIterator.hpp"
#include "helpers/BoardConstIterator.hpp"
#include "ai_classes/AICalculator.hpp"

#include "locators/Position.hpp"

#include <array>
#include <memory>

#include <thread>
#include <atomic>

#include "helpers/BoardSounds.hpp"

enum class PlayerTurn {
    White, Black
};

enum class Winner {
    White, Black, Draw, notFinished
};

class Board {
    std::unique_ptr<BoardDisplay> boardDisplay;
    std::unique_ptr<FenProcessor> fenProcessor;
    std::unique_ptr<ChessLogic> chessLogic;
    std::unique_ptr<AICalculator> aiCalculator;

    static const double DRAG_DISTANCE_LIMIT;


    bool firstRun = true;
    std::atomic<bool> moveMade = true;

    // Ai threads
    std::thread aiThread;
    std::atomic<bool> isAiThinking;

public:
    // --- Main Stuff ---

    sf::RenderWindow& window;
    const TextureManager& textureManager;
    BoardSounds& boardSounds;

    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> grid;
    float squareSize;
    Winner winnerValue = Winner::notFinished;

    // AI
    static constexpr bool aiActivated = false; // TODO add element to change this
    static constexpr int aiDepth = 3;
    static constexpr auto aiColour = PieceColour::Black;

    // stores the position of the possible enpassant, e.g. if you move pawn to e4 enpassant should generate at e3
    std::unique_ptr<Move> enpassantLocation;
    std::unique_ptr<Move> pawnUpgradedPosition;
    std::unique_ptr<Move> checkLocation;
    PieceColour playerTurn = PieceColour::White;
    int halfMoves = 0;
    int fullMoves = 0;

    std::unique_ptr<Move> pieceSelected;
    bool pieceBeingDragged = false;
    std::unique_ptr<Move> coordClicked;
    std::unique_ptr<Position> clickPosition;
    std::unique_ptr<std::array<Move, 2>> moveJustMade;

    Board(sf::RenderWindow& window, const TextureManager& textureManager, BoardSounds& boardSounds, float squareSize);

    Board(const Board &other);

    ~Board();

    BoardIterator begin();
    BoardIterator end();

    BoardConstIterator begin() const;
    BoardConstIterator end() const;

private:
    void checkIfDragged();
    void leftMousePressedLogic();
    void leftMouseReleasedLogic();
    void resetSelectionLogic();
    void pawnUpgradeSelectionLogic();

    void handleEvents();

    void startAiThread();

    void aiManagement();

public:

    // --- General Stuff ---

    MoveType getMoveType(const Piece *firstPiece, const Move &secondCoord);

    void checkForCheck(const Move &startCoord, const Move &endCoord);

    void calculateMoves(bool simulation) const;

    void swapPlayerTurn(bool simulation = false);

    Position getMousePos() const;
    Move getBoardCoord(const Position &pos) const;
    Move getBoardHoveredCoord() const;

    // Returns the piece and removes it from the board
    Piece popSquare(int x, int y);
    // Returns the piece and removes it from the board
    Piece popSquare(const Move& move);

    void setSquare(const Piece &piece, int x, int y);
    void setSquare(const Piece &piece, const Move &move);
    void setSquare(char pieceLetter, const Move &move);

    Piece *getPiece(int x, int y);
    [[nodiscard]] Piece *getPiece(int x, int y) const;
    Piece *getPiece(const Move& move);
    [[nodiscard]] Piece *getPiece(const Move& move) const;

    // --- Outside Access ---

    void setPosition(const std::string& fen = FenProcessor::STARTING_FEN) const;

    void draw() const;

    void print() const;

    std::string getFen() const;

    void makeMove(Piece *piece, const Move &moveToMake, bool simulation = false) const;

    void gameLoop();

    bool isCheckmate(bool staleMateFound = false) const;

    bool isStalemate() const;

    void isGameFinished();
};



#endif //BOARD_H
