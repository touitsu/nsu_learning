package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public abstract class Piece {
    protected Side side;
    protected PieceType type;

    public Piece(Side side, PieceType type) {
        this.side = side;
        this.type = type;
    }

    abstract public ArrayList<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position);

    protected boolean isMoveAvailable(@NotNull Board currentBoard, @NotNull Coordinates coordinates) {
        Piece piece;

        piece = currentBoard.getPieceAt(coordinates.x(), coordinates.y());

        return piece == null || (piece.side != this.side && piece.type != PieceType.King && piece.type != PieceType.OutOfBounds);
    }

    public Side getSide() {
        return side;
    }

    public PieceType getType() {
        return type;
    }
}
