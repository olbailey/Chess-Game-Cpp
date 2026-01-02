//
// Created by olbai on 23/07/2025.
//

#include "Board.hpp"

Position Board::getMousePos() const {
    return Position(sf::Mouse::getPosition(window));
}

Move Board::getBoardCoord(const Position& pos) const {
    return Move(std::array{pos.y / static_cast<int>(squareSize), pos.x / static_cast<int>(squareSize)});
}

Move Board::getBoardHoveredCoord() const {
    const Position& pos = getMousePos();
    return Move(std::array{pos.y / static_cast<int>(squareSize), pos.x / static_cast<int>(squareSize)});
}

MoveType Board::getMoveType(const Piece *firstPiece, const Move &secondCoord) {
    const Piece* secondPiece = getPiece(secondCoord);

    if (secondPiece == nullptr) return MoveType::Empty;

    if (firstPiece->getPieceColour() == secondPiece->getPieceColour()) return MoveType::Friendly;

    return MoveType::Attacking;
}

Piece Board::popSquare(const int x, const int y) {
    Piece pieceTemp = *getPiece(x, y);
    grid[x][y] = nullptr;
    return pieceTemp;
}
Piece Board::popSquare(const Move &move) {
    Piece pieceTemp = *getPiece(move);
    grid[move.x][move.y] = nullptr;
    return pieceTemp;
}

void Board::setSquare(const Piece &piece, const int x, const int y) {
    grid[x][y] = Piece::setPiece(piece.getPieceLetter(), Move({x, y}));
}

void Board::setSquare(const Piece &piece, const Move &move) {
    grid[move.x][move.y] = Piece::setPiece(piece.getPieceLetter(), Move({move.x, move.y}));
}

void Board::setSquare(const char pieceLetter, const Move &move) {
    grid[move.x][move.y] = Piece::setPiece(pieceLetter, Move({move.x, move.y}));
}

Piece *Board::getPiece(const int x, const int y) {return grid[x][y].get();}
Piece *Board::getPiece(const int x, const int y) const {return grid[x][y].get();}
Piece *Board::getPiece(const Move &move) {
    return grid[move.x][move.y].get();
}
Piece *Board::getPiece(const Move &move) const {
    return grid[move.x][move.y].get();
}

BoardIterator Board::begin() {return {this, 0, 0};}
BoardIterator Board::end() {return {this, 8, 0};}

BoardConstIterator Board::begin() const {return {this, 0, 0};}
BoardConstIterator Board::end() const {return {this, 8, 0};}