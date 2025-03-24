package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public final class Knight extends Piece {

    public Knight(Side side) {
        super(side, PieceType.Knight);
    }

    @Override
    public ArrayList<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        ArrayList<Coordinates> res;

        res = new ArrayList<Coordinates>();

        for (int x = 1, y = 2; x < 3 && y > 0; x++, y--) {
            if (isMoveAvailable(currentBoard, new Coordinates(position.x() + x, position.y() + y))) {
                res.add(new Coordinates(position.x() + x, position.y() + y));
            }

            if (isMoveAvailable(currentBoard, new Coordinates(position.x() - x, position.y() + y))) {
                res.add(new Coordinates(position.x() - x, position.y() + y));
            }

            if (isMoveAvailable(currentBoard, new Coordinates(position.x() + x, position.y() - y))) {
                res.add(new Coordinates(position.x() + x, position.y() - y));
            }

             if (isMoveAvailable(currentBoard, new Coordinates(position.x() - x, position.y() - y))) {
                res.add(new Coordinates(position.x() - x, position.y() - y));
            }
        }

        return res;
    }
}
