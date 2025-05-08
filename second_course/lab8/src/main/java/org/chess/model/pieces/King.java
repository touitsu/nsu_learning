package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class King extends Piece {

    private boolean moved = false;

    public King(@NotNull Side side, @NotNull Coordinates coordinates) {
        super(side, PieceType.King, coordinates);
    }

    @Override
    public HashSet<Coordinates> getMoves(@NotNull Board currentBoard) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        for (int i = this.coordinates.x() - 1; i < this.coordinates.x() + 2; i++) {
            for (int j = this.coordinates.y() - 1; j < this.coordinates.y() + 2; j++) {
                if ((i != this.coordinates.x() || j != this.coordinates.y()) && i >= 0 && i < 8 && j >=0 && j < 8) {
                    res.add(new Coordinates(i, j));
                }
            }
        }

        if (!moved) {
            if (currentBoard.getPieceAt(0, this.coordinates.y()) != null) {
                if (currentBoard.getPieceAt(0, this.coordinates.y()).getType() == PieceType.Rook &&
                        currentBoard.getPieceAt(0, this.coordinates.y()).getSide() == this.side) {
                    if (currentBoard.getPieceAt(1, this.coordinates.y()) == null && currentBoard.getPieceAt(2, this.coordinates.y()) == null && currentBoard.getPieceAt(3, this.coordinates.y()) == null) {
                        if (!((Rook) currentBoard.getPieceAt(0, this.coordinates.y())).getMoved()) {
                            res.add(new Coordinates(1, this.coordinates.y()));
                        }
                    }
                }
            }

            if (currentBoard.getPieceAt(7, this.coordinates.y()) != null) {
                if (currentBoard.getPieceAt(7, this.coordinates.y()).getType() == PieceType.Rook &&
                        currentBoard.getPieceAt(7, this.coordinates.y()).getSide() == this.side) {
                    if (currentBoard.getPieceAt(6, this.coordinates.y()) == null && currentBoard.getPieceAt(5, this.coordinates.y()) == null) {
                        if (!((Rook) currentBoard.getPieceAt(7, this.coordinates.y())).getMoved()) {
                            res.add(new Coordinates(6, this.coordinates.y()));
                        }
                    }
                }
            }
        }

        return res;
    }

    @Override
    public void move(Coordinates coordinates) {
        this.coordinates = coordinates;
        this.moved = true;
    }

}
