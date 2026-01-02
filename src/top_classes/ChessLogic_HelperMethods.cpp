//
// Created by olbai on 23/07/2025.
//

#include "ChessLogic.hpp"

#include "Board.hpp"

void ChessLogic::calculateCastles() const {
    // White queen side castle
    if (Piece *king = board_.getPiece(7, 4); castleIsPossible(board_, king, board_.getPiece(7, 0)))
        dynamic_cast<King*>(king)->addCastleMove(7, 2);
    // White king side castle
    if (Piece *king = board_.getPiece(7, 4); castleIsPossible(board_, king, board_.getPiece(7, 7)))
        dynamic_cast<King*>(king)->addCastleMove(7, 6);
    // Black queen side castle
    if (Piece *king = board_.getPiece(0, 4); castleIsPossible(board_, king, board_.getPiece(0, 0)))
        dynamic_cast<King*>(king)->addCastleMove(0, 2);
    // Black king side castle
    if (Piece *king = board_.getPiece(0, 4); castleIsPossible(board_, king, board_.getPiece(0, 7)))
        dynamic_cast<King*>(king)->addCastleMove(0, 6);
}

bool ChessLogic::gameEnded() const {
    for (const Piece *piece : board_) {
        if (piece->hasLegalMoves()) return false;
    }
    return true;
}


bool ChessLogic::validMove(const Move& pieceBeingMoved, const Move& placeMovedTo) {
    if (Piece *pieceToCheck = board_.getPiece(pieceBeingMoved);
            pieceToCheck != nullptr && pieceToCheck->hasMove(placeMovedTo)) {
        makeMove(pieceToCheck, pieceToCheck->getMove(placeMovedTo));
        return true;
    }
    return false;
}

bool ChessLogic::castleIsPossible(const Board &boardRef, Piece* kingPiece, Piece* rookPiece) {
    const King* kingPieceRawPtr = dynamic_cast<King*>(kingPiece);
    const Rook* rookPieceRawPtr = dynamic_cast<Rook*>(rookPiece);

    if (kingPieceRawPtr == nullptr || rookPieceRawPtr == nullptr) return false;
    if (kingPieceRawPtr->getPieceType() != PieceType::King) return false;
    if (rookPieceRawPtr->getPieceType() != PieceType::Rook) return false;
    if (kingPieceRawPtr->getPieceColour() != rookPieceRawPtr->getPieceColour()) return false;
    if (kingPieceRawPtr->getBeenMoved() || rookPieceRawPtr->getBeenMoved()) return false;

    // checking if white king and rook can be in acceptable positions
    if (kingPieceRawPtr->getPieceColour() == PieceColour::White
            && kingPieceRawPtr->getPiecePosition().coordEqualTo(Move({7, 4}))) {

        if (rookPieceRawPtr->getPiecePosition().coordEqualTo(Move({7, 0})) // if rook is on the left side
                && boardRef.getPiece(7, 1) == nullptr && boardRef.getPiece(7, 2) == nullptr
                && boardRef.getPiece(7, 3) == nullptr // if spaces are empty between rook on left and king
            || (rookPieceRawPtr->getPiecePosition().coordEqualTo(Move({7, 7})) // if rook is on the right side
                && boardRef.getPiece(7, 5) == nullptr && boardRef.getPiece(7, 6) == nullptr)) // if spaces are empty between rook on right and king
            return true;

    // checking if black king and rook can be in acceptable positions
    } else if (kingPieceRawPtr->getPiecePosition().coordEqualTo(Move({0, 4}))) {

        if (rookPieceRawPtr->getPiecePosition().coordEqualTo(Move({0, 0})) // if rook is on the left side
                && boardRef.getPiece(0, 1) == nullptr && boardRef.getPiece(0, 2) == nullptr
                && boardRef.getPiece(0, 3)  == nullptr// if spaces are empty between rook on left and king
            || (rookPieceRawPtr->getPiecePosition().coordEqualTo(Move({0, 7})) // if rook is on the right side
                && boardRef.getPiece(0, 5) == nullptr && boardRef.getPiece(0, 6) == nullptr)) // if spaces are empty between rook on right and king
            return true;
    }
    return false;
}

bool ChessLogic::playerInCheck(const Board &board, const PieceColour &playerColour) {
    const Move& kingPosition = findKingPosition(board, playerColour);

    return squareBeingAttacked(board, playerColour, kingPosition);
}

bool ChessLogic::squareBeingAttacked(const Board &board, const PieceColour &playerColour, const Move &square) {
    for (const Piece* piecePtr : board) {
        if (piecePtr != nullptr
                && piecePtr->getPieceColour() != playerColour && piecePtr->isAttackingPosition(square)) {
            return true;
                }
    }

    return false;
}


const Move& ChessLogic::findKingPosition(const Board &boardCopy, const PieceColour &pieceColour) {
    for (const Piece* piecePtr : boardCopy) {
        if (piecePtr != nullptr && piecePtr->getPieceType() == PieceType::King && piecePtr->getPieceColour() == pieceColour) {
            return piecePtr->getPiecePosition();
        }
    }

    throw std::runtime_error("King not found");
}

void ChessLogic::moveMadeLogic(Piece &piece, const Move &moveToMake) const {
    piece.setBeenMoved(true);
    board_.enpassantLocation.reset();
    board_.moveJustMade = std::make_unique<std::array<Move, 2>>(std::array{
            Move::copy(piece.getPiecePosition()),
            Move::copy(moveToMake)}
        );

    if (piece.getPieceType() == PieceType::Pawn) {
        const Move &currentPosition = piece.getPiecePosition();

        if (moveToMake.type == MoveType::DoubleMove) {
            board_.enpassantLocation = std::make_unique<Move>(
                Move({currentPosition.x + (moveToMake.x - currentPosition.x) / 2, moveToMake.y}));

        } else if (const PieceColour &pieceColour = piece.getPieceColour();
                (currentPosition.x == 1 && pieceColour == PieceColour::White)
                || (currentPosition.x == 6 && pieceColour == PieceColour::Black)) {
            board_.pawnUpgradedPosition = std::make_unique<Move>(Move(moveToMake));
        }
    }
}
