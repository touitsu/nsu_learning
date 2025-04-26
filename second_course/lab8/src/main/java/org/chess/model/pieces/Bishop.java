package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class Bishop extends Piece {

    public Bishop(@NotNull Side side, @NotNull Coordinates coordinates) {
        super(side, PieceType.Bishop, coordinates);
    }

    @Override
    public HashSet<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && inBounds(position.x() + i, position.y() + i); i++) {
            res.add(new Coordinates(position.x() + i, position.y() + i));
            if (currentBoard.getPieceAt(position.x() + i, position.y() + i) != null) {
                breakFlag = 1;
            }
        }

        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && inBounds(position.x() + i, position.y() - i); i++) {
            res.add(new Coordinates(position.x() + i, position.y() - i));
            if (currentBoard.getPieceAt(position.x() + i, position.y() - i) != null) {
                breakFlag = 1;
            }
        }

        for (int i = 1, breakFlag = 0; i < 8 && breakFlag == 0 && inBounds(position.x() - i, position.y() + i); i++) {
            res.add(new Coordinates(position.x() - i, position.y() + i));
            if (currentBoard.getPieceAt(position.x() - i, position.y() + i) != null) {
                breakFlag = 1;
            }
        }

        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && inBounds(position.x() - i, position.y() - i); i++) {
            res.add(new Coordinates(position.x() - i, position.y() - i));
            if (currentBoard.getPieceAt(position.x() - i, position.y() - i) != null) {
                breakFlag = 1;
            }
        }

        return res;
    }

}
