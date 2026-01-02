//
// Created by olbai on 30/06/2025.
//

#include "Piece.hpp"

#include "pieces/Pawn.hpp"
#include "pieces/Knight.hpp"
#include "pieces/Bishop.hpp"
#include "pieces/Rook.hpp"
#include "pieces/Queen.hpp"
#include "pieces/King.hpp"

#include "../../include/helpers/TextureManager.hpp"

#include <iostream>
#include <memory>

#include "Board.hpp"

const std::vector<MoveType> Piece::POSSIBLE_MOVETYPES = {
    MoveType::Empty, MoveType::Attacking,
    MoveType::SingleMove, MoveType::DoubleMove,
    MoveType::KingMove, MoveType::Castling,
    MoveType::Enpassant, MoveType::PromotionQueen,
    MoveType::PromotionKnight, MoveType::PromotionBishop, MoveType::PromotionRook
};

const std::vector<MoveType> Piece::DRAWABLE_MOVETYPES = {
    MoveType::Empty, MoveType::Attacking,
    MoveType::SingleMove, MoveType::DoubleMove,
    MoveType::KingMove, MoveType::Castling,
    MoveType::Enpassant
};

Piece::Piece(const PieceType type, const PieceColour colour, const Move &position)
    : type(type), colour(colour), currentPosition(position) {}

void Piece::calculateMoves(Board &board) {
    std::cerr << "Somehow base Piece calculateMoves method got called! \n" << std::endl;
}

bool Piece::isAttackingPosition(const Move& coord) const {
    std::cerr << "Somehow base Piece isAttackingPosition method got called!" << std::endl;
    return false;
}

void Piece::removeIllegalMoves(const Board &board) {
    const std::vector<Move> movesCopy = moves;
    moves.clear();
    for (const Move& move : movesCopy) {
        if (!ChessLogic::illegalMove(board, currentPosition, move)) {
            moves.emplace_back(move, move.type);
        } else if (type != PieceType::Pawn
               || move.type != MoveType::SingleMove && move.type != MoveType::DoubleMove) {
            moves.emplace_back(move, MoveType::None);
        }
    }
}

std::vector<Move> Piece::getPossibleMoves(const std::vector<MoveType> &moveTypesToShow) const {
    std::vector<Move> possibleMoves;

    for (const Move &move : moves) {
        if (Move::moveTypeInVector(moveTypesToShow, move.type)) possibleMoves.emplace_back(move);
    }

    return possibleMoves;
}

bool Piece::hasMove(const Move &possibleMove) const {
    for (const Move &move : moves) {
        if (move.type != MoveType::Friendly && move.type != MoveType::None
            && move.coordEqualTo(possibleMove)) return true;
    }
    return false;
}

Move Piece::getMove(const Move &moveToGet) const {
    for (const Move &move : moves) {
        if (moveToGet.coordEqualTo(move)) return move;
    }
    return Move({-1, -1}, MoveType::None);
}


bool Piece::hasLegalMoves() const {
    for (const Move &move : moves) {
        if (move.type != MoveType::Friendly) return true;
    }
    return false;
}

bool Piece::MovesCollisionCheck(const std::vector<Move> &moves, const Move &coordToCheck) {
    for (const Move &move : moves) {
        if (move.coordEqualTo(coordToCheck)
                && (move.type == MoveType::Empty || move.type == MoveType::Friendly
                    || move.type == MoveType::Attacking || move.type == MoveType::KingMove || move.type == MoveType::None))
            return true;
    }

    return false;
}

char Piece::getPieceLetter() const {
    return pieceToLetter(*this);
}

std::vector<Move> Piece::getMoves() const {return moves;}

void Piece::setMoves(const std::vector<Move>& otherMoves) {
    moves = otherMoves;
}

const PieceType &Piece::getPieceType() const {return type;}
const PieceColour &Piece::getPieceColour() const {return colour;}
const Move &Piece::getPiecePosition() const {return currentPosition;}

std::ostream &Piece::print(std::ostream &os) const {
    os << type  << " (" << currentPosition.x << ", " << currentPosition.y<< ") : ";

    for (const Move &move : moves) {
        os << move << ", ";
    }

    return os;
}

bool Piece::getBeenMoved() const {return false;}
void Piece::setBeenMoved(const bool value) {}

std::ostream &operator<<(std::ostream &os, const Piece &piece) {
    return piece.print(os);
}