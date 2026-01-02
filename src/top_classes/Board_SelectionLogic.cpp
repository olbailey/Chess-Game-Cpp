//
// Created by olbai on 23/07/2025.
//

#include "Board.hpp"

const double Board::DRAG_DISTANCE_LIMIT = 10;

void Board::leftMousePressedLogic() {
    if ( isAiThinking || (aiActivated && aiColour == playerTurn)) return;

    clickPosition = std::make_unique<Position>(getMousePos());
    coordClicked = std::make_unique<Move>(getBoardCoord(*clickPosition));
    const Piece* possiblePiece = getPiece(*coordClicked);

    if (pawnUpgradedPosition != nullptr) {
        pawnUpgradeSelectionLogic();
        return;
    }

    if (pieceSelected == nullptr) {
        if (possiblePiece == nullptr || possiblePiece->getPieceColour() != playerTurn) {
            resetSelectionLogic();
            return; // if a piece who does not belong to the current player is clicked
        }

        return; // skips it for some reason
    }

    if (possiblePiece != nullptr && possiblePiece->getPieceColour() == getPiece(*pieceSelected)->getPieceColour()) {
        pieceSelected.reset();
        return; // if player clicks on a friendly piece when already having selected a piece
    }

    if (chessLogic->validMove(*pieceSelected, *coordClicked)) {
        moveMade = true;
    }
    resetSelectionLogic();
}

void Board::leftMouseReleasedLogic() {
    if (isAiThinking || (aiActivated && aiColour == playerTurn)) return;

    if (pieceSelected != nullptr || pawnUpgradedPosition != nullptr) return;

    if (coordClicked == nullptr || clickPosition == nullptr) {
        resetSelectionLogic();
        return;
    }
    if (const Piece* possiblePiece = getPiece(*coordClicked);
            possiblePiece->getPieceColour() != playerTurn) {
        resetSelectionLogic();
        return;
    }

    if (const Move &moveto = getBoardCoord(getMousePos());
            coordClicked->coordEqualTo(moveto)) {
        pieceSelected = std::make_unique<Move>(moveto);
        pieceBeingDragged = false;

    } else if (pieceBeingDragged) {
        if (chessLogic->validMove(*coordClicked, getBoardHoveredCoord())) {
            moveMade = true;
        }
        resetSelectionLogic();
    }

}

void Board::checkIfDragged() {
    if (clickPosition != nullptr && pieceSelected == nullptr
            && getBoardCoord(*clickPosition).toPosition(squareSize, true).distanceBetween(getMousePos()) > DRAG_DISTANCE_LIMIT) {
        pieceBeingDragged = true;
    }
    else
        pieceBeingDragged = false;
}

void Board::pawnUpgradeSelectionLogic() {
    const Move &pawnCoord = *pawnUpgradedPosition;
    Piece *pawnBeingUpgraded = getPiece(pawnCoord);
    const PieceColour &pawnColour = pawnBeingUpgraded->getPieceColour();
    const int directionMultiplier = (pawnColour == PieceColour::White) ? 1 : -1;

    for (int i = 0; i < 4; ++i) {
        const Move squareCoord({pawnCoord.x + i * directionMultiplier, pawnCoord.y});
        if (const Move currentMouseCoord = getBoardCoord(getMousePos());
                currentMouseCoord.coordEqualTo(squareCoord)) {
            chessLogic->makeMove(pawnBeingUpgraded, Move(pawnBeingUpgraded->getPiecePosition(), Move::getPromotionType(i)));
            moveMade = true;
            swapPlayerTurn();
        }
    }
    resetSelectionLogic();
}


void Board::resetSelectionLogic() {
    clickPosition.reset();
    coordClicked.reset();
    pieceSelected.reset();
    pieceBeingDragged = false;
}

void Board::swapPlayerTurn(const bool simulation) {
    if (firstRun && !simulation) {
        firstRun = false;
        return;
    }

    checkLocation.reset();

    if (playerTurn == PieceColour::White) {
        playerTurn = PieceColour::Black;
    } else {
        playerTurn = PieceColour::White;
        fullMoves++;
    }
}