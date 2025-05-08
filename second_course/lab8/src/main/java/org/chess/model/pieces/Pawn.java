package org.chess.model.pieces;

import org.chess.model.*;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class Pawn extends Piece {

    public Pawn(@NotNull Side side, Coordinates coordinates) {
        super(side, PieceType.Pawn, coordinates);
    }

    @Override
    public HashSet<Coordinates> getMoves(@NotNull Board currentBoard) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        if (currentBoard.getPieceAt(this.coordinates.x(), this.coordinates.y() + this.side.getValue()) == null &&
                (this.coordinates.y() + this.side.getValue()) < 8 &&
                (this.coordinates.y() + this.side.getValue()) > 0)  {

            res.add(new Coordinates(this.coordinates.x(), this.coordinates.y() + this.side.getValue()));
        }

        if (((this.coordinates.y() == 1 && super.side == Side.WHITE) || (this.coordinates.y() == 6 && this.side == Side.BLACK)) &&
                currentBoard.getPieceAt(this.coordinates.x(), this.coordinates.y() + 2 * this.side.getValue()) == null &&
                currentBoard.getPieceAt(this.coordinates.x(), this.coordinates.y() + this.side.getValue()) == null) {

            res.add(new Coordinates(this.coordinates.x(), this.coordinates.y() + 2 * super.side.getValue()));
        }

        res.add(new Coordinates(this.coordinates.x() + 1, this.coordinates.y() + super.side.getValue()));
        res.add(new Coordinates(this.coordinates.x() - 1, this.coordinates.y() + super.side.getValue()));

        return res;
    }
}
