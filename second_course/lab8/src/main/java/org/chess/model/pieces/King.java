package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public final class King extends Piece {

    public King(Side side) {
        super(side, PieceType.King);
    }

    @Override
    public ArrayList<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        ArrayList<Coordinates> res;

        res = new ArrayList<Coordinates>();

        for (int i = position.x() - 1; i < position.x() + 2; i++) {
            for (int j = position.y() - 1; j < position.y() + 2; j++) {
                if (i != position.x() && j != position.y()) {
                    if (isMoveAvailable(currentBoard, new Coordinates(position.x() + i, position.y() + j))) {
                        res.add(new Coordinates(i, j));
                    }
                }
            }
        }

        return res;
    }

}
