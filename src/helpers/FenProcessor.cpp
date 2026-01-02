//
// Created by olbai on 04/07/2025.
//

#include "helpers/FenProcessor.hpp"

#include "Board.hpp"
#include "Piece.hpp"
#include "ChessLogic.hpp"

#include "Pieces/King.hpp"
#include "Pieces/Rook.hpp"

const std::string FenProcessor::STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

FenProcessor::FenProcessor(Board &board)
    : board(board) {}


void FenProcessor::setBoard(const std::string& fen) const {
    std::stringstream fenStream(fen);

    std::string fenSection;

    if (std::getline(fenStream, fenSection, ' '))
        setPieces(fenSection);

    if (std::getline(fenStream, fenSection, ' '))
        if (fenSection == "w")
            board.playerTurn = PieceColour::White;
        else
            board.playerTurn = PieceColour::Black;

    if (std::getline(fenStream, fenSection, ' ') && fenSection != "-")
        setCastling(fenSection);

    if (std::getline(fenStream, fenSection, ' ') && fenSection != "-")
        board.enpassantLocation = std::make_unique<Move>(Move::notationToPosition(fenSection));

    if (std::getline(fenStream, fenSection, ' '))
        board.halfMoves = std::stoi(fenSection);

    if (std::getline(fenStream, fenSection, ' '))
        board.fullMoves = std::stoi(fenSection);
}

void FenProcessor::setPieces(const std::string& BoardFenSection) const {
    std::stringstream pieceStream(BoardFenSection);
    std::string line;

    for (int i = 0; i < 8; ++i) {
        std::getline(pieceStream, line, '/');
        int j = 0;

        for (const char pieceChar : line) {
            if (std::isdigit(pieceChar)) {
                // a way of converting char to int without it returning ascii value
                const int emptySpacesCount = pieceChar - '0';

                j += emptySpacesCount;
            } else {
                board.grid[i][j] = Piece::setPiece(pieceChar, Move({i, j}));
                j++;
            }
        }
    }
}

void FenProcessor::setCastling(const std::string &castlingFenSection) const {
    std::stringstream castlingStream(castlingFenSection);
    char castlingLetter;

    King* kingPieceRawPtr = nullptr;
    Rook* rookPieceRawPtr = nullptr;

    while (castlingStream >> castlingLetter) {
        switch (castlingLetter) {
            case 'K':
                kingPieceRawPtr = dynamic_cast<King*>(board.getPiece(7, 4));
                rookPieceRawPtr = dynamic_cast<Rook*>(board.getPiece(7, 7));
                checkKingRookPair(kingPieceRawPtr, rookPieceRawPtr, PieceColour::White);
                break;

            case 'Q':
                kingPieceRawPtr = dynamic_cast<King*>(board.getPiece(7, 4));
                rookPieceRawPtr = dynamic_cast<Rook*>(board.getPiece(7, 0));
                checkKingRookPair(kingPieceRawPtr, rookPieceRawPtr, PieceColour::White);
                break;

            case 'k':
                kingPieceRawPtr = dynamic_cast<King*>(board.getPiece(0, 4));
                rookPieceRawPtr = dynamic_cast<Rook*>(board.getPiece(0, 7));
                checkKingRookPair(kingPieceRawPtr, rookPieceRawPtr, PieceColour::Black);
                break;

            case 'q':
                kingPieceRawPtr = dynamic_cast<King*>(board.getPiece(0, 4));
                rookPieceRawPtr = dynamic_cast<Rook*>(board.getPiece(0, 0));
                checkKingRookPair(kingPieceRawPtr, rookPieceRawPtr, PieceColour::Black);
                break;

            default:
                throw std::invalid_argument("Error parsing castling fen input!: " + castlingLetter);
        }
    }
}

void FenProcessor::checkKingRookPair(King *kingPtr, Rook *rookPtr, const PieceColour& pieceColour) {
    if (kingPtr != nullptr && rookPtr != nullptr
        && kingPtr->getPieceType() == PieceType::King
        && rookPtr->getPieceType() == PieceType::Rook
        && kingPtr->getPieceColour() == rookPtr->getPieceColour()
        && kingPtr-> getPieceColour() == pieceColour) {
        kingPtr->setBeenMoved(false);
        rookPtr->setBeenMoved(false);
    }
}


std::string FenProcessor::boardToFen() const {
    std::stringstream fenStream;

    fenStream << piecesForFen();

    if (board.playerTurn == PieceColour::White)
        fenStream << " w ";
    else
        fenStream << " b ";

    fenStream << castlingForFen();

    if (board.enpassantLocation == nullptr) fenStream << " -";
    else fenStream << ' ' << board.enpassantLocation->getNotation();

    fenStream << ' ' << board.halfMoves;
    fenStream << ' ' << board.fullMoves;

    return fenStream.str();
}

std::string FenProcessor::piecesForFen() const {
    std::stringstream piecesFenStream;
    int emptySpacesCount = 0;

    for (int i = 0; i < 8; ++i) {
        for (const auto& piecePtr : board.grid[i]) {
            if (piecePtr == nullptr) emptySpacesCount++;

            else {
                if (emptySpacesCount != 0) {
                    piecesFenStream << emptySpacesCount;
                    emptySpacesCount = 0; }

                piecesFenStream << Piece::pieceToLetter(*piecePtr);
            }
        }

        if (emptySpacesCount != 0) {
            piecesFenStream << emptySpacesCount;
            emptySpacesCount = 0; }

        if (i != 7) piecesFenStream << '/';
    }

    return piecesFenStream.str();
}

std::string FenProcessor::castlingForFen() const {
    std::stringstream castlingStream;

    // White king side castle
    if (ChessLogic::castleIsPossible(board, board.getPiece(7, 4), board.getPiece(7, 7)))
        castlingStream << 'K';
    // White queen side castle
    if (ChessLogic::castleIsPossible(board, board.getPiece(7, 4), board.getPiece(7, 0)))
        castlingStream << 'Q';
    // Black king side castle
    if (ChessLogic::castleIsPossible(board, board.getPiece(0, 4), board.getPiece(0, 7)))
        castlingStream << 'k';
    // Black queen side castle
    if (ChessLogic::castleIsPossible(board, board.getPiece(0, 4), board.getPiece(0, 0)))
        castlingStream << 'q';

    if (castlingStream.str() == "") castlingStream << '-';

    return castlingStream.str();
}
