package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class King extends Piece {

    public King(@NotNull Side side, @NotNull Coordinates coordinates) {
        super(side, PieceType.King, coordinates);
    }

    @Override
    public HashSet<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        for (int i = position.x() - 1; i < position.x() + 2; i++) {
            for (int j = position.y() - 1; j < position.y() + 2; j++) {
                if (i != position.x() || j != position.y()) {
                    res.add(new Coordinates(i, j));
                }
            }
        }

        return res;
    }

}
