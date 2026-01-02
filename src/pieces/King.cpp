//
// Created by olbai on 30/06/2025.
//

#include "pieces/King.hpp"

#include <array>

#include "Board.hpp"

King::King(const PieceType type, const PieceColour colour, const Move position, const bool beenMoved)
    : Piece(type, colour, position), beenMoved(beenMoved) {
}

void King::calculateMoves(Board &board) {
    moves.clear();

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if ((i != 0 || j != 0) // so it doesn't check the position of the king
                    && 0 <= currentPosition.x + i && currentPosition.x + i <= 7
                    && 0 <= currentPosition.y + j && currentPosition.y + j <= 7) {
                const MoveType moveType = board.getMoveType(this,
                    Move({currentPosition.x + i, currentPosition.y + j}));

                if (moveType == MoveType::Empty)
                    moves.emplace_back(std::array{currentPosition.x + i, currentPosition.y + j}, MoveType::KingMove);
                else
                    moves.emplace_back(std::array{currentPosition.x + i, currentPosition.y + j}, moveType);
            }
        }
    }
}

bool King::isAttackingPosition(const Move &coord) const {
    std::vector<Move> movesCopy = moves;

    for (const Move& move : moves) {
        if (move.type == MoveType::Castling);
    }

    return MovesCollisionCheck(moves, coord);
}

void King::addCastleMove(const int x, const int y) {
    moves.emplace_back(std::array{x, y}, MoveType::Castling);
}

bool King::getBeenMoved() const {return beenMoved;}
void King::setBeenMoved(const bool newBeenMoved) {beenMoved = newBeenMoved;}