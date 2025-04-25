package org.chess.model.pieces;

import org.chess.model.*;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public final class Pawn extends Piece {

    public Pawn(@NotNull Side side, Coordinates coordinates) {
        super(side, PieceType.Pawn, coordinates);
    }

    @Override
    public HashSet<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        HashSet<Coordinates> res;

        res = new HashSet<Coordinates>();

        if (currentBoard.getPieceAt(position.x(), position.y() + this.side.getValue()) == null &&
                (position.y() + this.side.getValue()) < 8 &&
                (position.y() + this.side.getValue()) > 0)  {

            res.add(new Coordinates(position.x(), position.y() + this.side.getValue()));
        }

        if (((position.y() == 1 && super.side == Side.WHITE) || (position.y() == 6 && this.side == Side.BLACK)) &&
                currentBoard.getPieceAt(position.x(), position.y() + 2 * this.side.getValue()) == null &&
                currentBoard.getPieceAt(position.x(), position.y() + this.side.getValue()) == null) {

            res.add(new Coordinates(position.x(), position.y() + 2 * super.side.getValue()));
        }

        res.add(new Coordinates(position.x() + 1, position.y() + super.side.getValue()));
        res.add(new Coordinates(position.x() - 1, position.y() + super.side.getValue()));

        return res;
    }
}
