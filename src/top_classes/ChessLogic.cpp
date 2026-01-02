//
// Created by olbai on 04/07/2025.
//

#include "ChessLogic.hpp"

#include "Board.hpp"

#include <array>

ChessLogic::ChessLogic(Board &board)
    : board_(board) {}


void ChessLogic::calculateMoves(const bool simulation) const {
    for (Piece* piecePtr : board_)
        if (piecePtr != nullptr)
            piecePtr->calculateMoves(board_);

    calculateCastles();

    if (simulation) return;

    for (Piece* piecePtr : board_)
        if (piecePtr != nullptr)
            piecePtr->removeIllegalMoves(board_);
}

void ChessLogic::makeMove(Piece *piece, const Move &moveToMake, const bool simulation) const {
    moveMadeLogic(*piece, moveToMake);

    const Move currentPosition = Move::copy(piece->getPiecePosition());

    if (moveToMake.type == MoveType::Castling) {
        const Piece& kingTemp = board_.popSquare(currentPosition);
        board_.setSquare(kingTemp, moveToMake);

        if (const int xDifference = moveToMake.y - currentPosition.y;
            xDifference < 0) { // queen side castle

            const Piece &rookTemp = board_.popSquare(Move({currentPosition.x, 0}));
            board_.setSquare(rookTemp, Move({currentPosition.x, moveToMake.y + 1}));
            }
        else { // king side castle

            const Piece &rookTemp = board_.popSquare(Move({currentPosition.x, 7}));
            board_.setSquare(rookTemp, Move({currentPosition.x, moveToMake.y - 1}));
        }
        if (!simulation)
            board_.boardSounds.requestSound("castle");
        return;
    }

    if (moveToMake.type == MoveType::Enpassant) {
        const Piece& pawnTemp = board_.popSquare(currentPosition);
        board_.setSquare(pawnTemp, moveToMake);
        board_.popSquare(Move({currentPosition.x, moveToMake.y}));
        if (!simulation)
            board_.boardSounds.requestSound("move");
        return;
    }

    if (moveToMake.isPromotion()) {
        board_.setSquare(Piece::getPawnPromotionLetter(moveToMake.type, piece->getPieceColour()), moveToMake);
        board_.popSquare(currentPosition);
        board_.pawnUpgradedPosition.reset();
        if (!simulation)
            board_.boardSounds.requestSound("promote");
        return;
    }

    // Reached if just a normal move

    const Piece& pieceTemp = board_.popSquare(currentPosition);
    board_.setSquare(pieceTemp, moveToMake);
    if (!simulation) {
        if (moveToMake.type == MoveType::Attacking)
            board_.boardSounds.requestSound("capture");
        else
            board_.boardSounds.requestSound("move");
    }
}

bool ChessLogic::illegalMove(Board board, const Move &startCoord, const Move &moveToMake) {
    Piece *attackingPiece = board.getPiece(startCoord);
    const MoveType &moveType = moveToMake.type;

    if (attackingPiece == nullptr) {
        std::cerr << "illegal moved Fucked \n";
        return false;
    }

    const PieceColour pieceColour = attackingPiece->getPieceColour();
    // If attacking piece is a king only matters if end coord is being attacked
    if (attackingPiece->getPieceType() == PieceType::King) {
        if (moveType == MoveType::Castling) { // unless checking a castle move
            if (squareBeingAttacked(board, pieceColour, startCoord)) return true; // if king is being attacked then castling not possible
            if (const int xDifference = moveToMake.y - startCoord.y;
                xDifference < 0) {
                // queen side castle
                if (squareBeingAttacked(board, pieceColour, Move({startCoord.x, startCoord.y - 1}))) return true;
            } else // king side castle
                if (squareBeingAttacked(board, pieceColour, Move({startCoord.x, startCoord.y + 1}))) return true;
        }

        return squareBeingAttacked(board, pieceColour, Move({moveToMake.x, moveToMake.y}));
    }



    // Normal move
    if (const Piece *attackedPiece = board.getPiece(moveToMake); // removes friendly attacks on kings
        attackedPiece != nullptr && moveType == MoveType::Friendly // because there is no point in that obvs
            && attackedPiece->getPieceType() == PieceType::King) return true;

    board.makeMove(attackingPiece, moveToMake, true);

    board.calculateMoves(true);

    return playerInCheck(board, pieceColour);
}