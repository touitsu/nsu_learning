package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public final class OutOfBounds extends Piece {

    public OutOfBounds() {
        super(Side.WHITE, PieceType.OutOfBounds);
    }

    @Override
    public ArrayList<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        return null;
    }
}
