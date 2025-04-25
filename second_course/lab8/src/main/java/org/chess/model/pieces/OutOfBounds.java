package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class OutOfBounds extends Piece {

    public OutOfBounds(@NotNull Coordinates coordinates) {
        super(Side.WHITE, PieceType.OutOfBounds, coordinates);
    }

    @Override
    public HashSet<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        return null;
    }
}
