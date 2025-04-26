package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class Rook extends Piece {

    public Rook(@NotNull Side side, @NotNull Coordinates coordinates) {
        super(side, PieceType.Rook, coordinates);
    }

    @Override
    public HashSet<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && position.x() + i < 8; i++) {
            res.add(new Coordinates(position.x() + i, position.y()));
            if (currentBoard.getPieceAt(position.x() + i, position.y()) != null) {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && position.x() - i >= 0; i++) {
            res.add(new Coordinates(position.x() - i, position.y() ));
            if (currentBoard.getPieceAt(position.x() - i, position.y()) != null) {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && position.y() + i < 8; i++) {
            res.add(new Coordinates(position.x(), position.y() + i));
            if (currentBoard.getPieceAt(position.x(), position.y() + i) != null) {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && position.y() - i >= 0; i++) {
            res.add(new Coordinates(position.x(), position.y() - i));
            if (currentBoard.getPieceAt(position.x(), position.y() - i) != null) {
                breakFlag = 1;
            }
        }

        return res;
    }
}
