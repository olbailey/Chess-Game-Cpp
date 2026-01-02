//
// Created by olbai on 04/07/2025.
//

#ifndef CHESSLOGIC_H
#define CHESSLOGIC_H

#include "Piece.hpp"
#include "pieces/King.hpp"

class Board;

class ChessLogic {
    Board& board_;

public:
    explicit ChessLogic(Board& board);

    void calculateMoves(bool simulation) const;

    void makeMove(Piece *piece, const Move &moveToMake, bool simulation = false) const;

    bool validMove(const Move &pieceBeingMoved, const Move &placeMovedTo);

    static bool illegalMove(Board board, const Move &startCoord, const Move &moveToMake);

    static bool castleIsPossible(const Board &boardRef, Piece* kingPiece, Piece* rookPiece);

private:
    void calculateCastles() const;

    bool gameEnded() const;

    static bool playerInCheck(const Board &board, const PieceColour& playerColour);

    static bool squareBeingAttacked(const Board &board, const PieceColour& playerColour, const Move &square);

    static const Move& findKingPosition(const Board &boardCopy, const PieceColour &pieceColour);

    void moveMadeLogic(Piece &piece, const Move &moveToMake) const;
};



#endif //CHESSLOGIC_H
