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
    public HashSet<Coordinates> getMoves(@NotNull Board currentBoard) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        for (int x = 1, y = 2; x < 3 && y > 0; x++, y--) {
            if (inBounds(this.coordinates.x() + x, this.coordinates.y() + y)) {
                res.add(new Coordinates(this.coordinates.x() + x, this.coordinates.y() + y));
            }

            if (inBounds(this.coordinates.x() - x, this.coordinates.y() + y)) {
                res.add(new Coordinates(this.coordinates.x() - x, this.coordinates.y() + y));
            }

            if (inBounds(this.coordinates.x() + x, this.coordinates.y() - y)) {
                res.add(new Coordinates(this.coordinates.x() + x, this.coordinates.y() - y));
            }

            if (inBounds(this.coordinates.x() - x, this.coordinates.y() - y)) {
                res.add(new Coordinates(this.coordinates.x() - x, this.coordinates.y() - y));
            }
        }

        return res;
    }
}
