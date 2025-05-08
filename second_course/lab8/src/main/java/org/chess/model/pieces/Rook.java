package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class Rook extends Piece {

    private boolean moved = false;

    public Rook(@NotNull Side side, @NotNull Coordinates coordinates) {
        super(side, PieceType.Rook, coordinates);
    }

    @Override
    public HashSet<Coordinates> getMoves(@NotNull Board currentBoard) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && this.coordinates.x() + i < 8; i++) {
            res.add(new Coordinates(this.coordinates.x() + i, this.coordinates.y()));
            if (currentBoard.getPieceAt(this.coordinates.x() + i, this.coordinates.y()) != null) {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && this.coordinates.x() - i >= 0; i++) {
            res.add(new Coordinates(this.coordinates.x() - i, this.coordinates.y() ));
            if (currentBoard.getPieceAt(this.coordinates.x() - i, this.coordinates.y()) != null) {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && this.coordinates.y() + i < 8; i++) {
            res.add(new Coordinates(this.coordinates.x(), this.coordinates.y() + i));
            if (currentBoard.getPieceAt(this.coordinates.x(), this.coordinates.y() + i) != null) {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1 && this.coordinates.y() - i >= 0; i++) {
            res.add(new Coordinates(this.coordinates.x(), this.coordinates.y() - i));
            if (currentBoard.getPieceAt(this.coordinates.x(), this.coordinates.y() - i) != null) {
                breakFlag = 1;
            }
        }

        return res;
    }

    @Override
    public void move(Coordinates coordinates) {
        this.coordinates = coordinates;
        this.moved = true;
    }

    public boolean getMoved() {
        return this.moved;
    }
}
