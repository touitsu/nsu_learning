package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.HashSet;

public abstract class Piece {
    protected final Side side;
    protected final PieceType type;
    protected Coordinates coordinates;

    public Piece(@NotNull Side side, @NotNull PieceType type, @NotNull Coordinates coordinates) {
        this.side = side;
        this.type = type;
        this.coordinates = coordinates;
    }

    abstract public HashSet<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position);

    public Side getSide() {
        return this.side;
    }

    public PieceType getType() {
        return this.type;
    }

    public Coordinates getCoordinates() {
        return this.coordinates;
    }

    public void move(Coordinates coordinates) {
        this.coordinates = coordinates;
    }

    public boolean inBounds(int x, int y) {
        return x >= 0 && x < 8 && y >= 0 && y < 8;
    }

    @Override
    public int hashCode() {
        return this.coordinates.x() * 1000 + this.coordinates.y() * 100 + this.type.getValue() * 10 + this.side.getValue();
    }
}
