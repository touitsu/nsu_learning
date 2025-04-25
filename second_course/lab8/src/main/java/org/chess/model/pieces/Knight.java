package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class Knight extends Piece {

    public Knight(@NotNull Side side, @NotNull Coordinates coordinates) {
        super(side, PieceType.Knight, coordinates);
    }

    @Override
    public HashSet<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        for (int x = 1, y = 2; x < 3 && y > 0; x++, y--) {
            res.add(new Coordinates(position.x() + x, position.y() + y));

            res.add(new Coordinates(position.x() - x, position.y() + y));

            res.add(new Coordinates(position.x() + x, position.y() - y));

            res.add(new Coordinates(position.x() - x, position.y() - y));
        }

        return res;
    }
}
