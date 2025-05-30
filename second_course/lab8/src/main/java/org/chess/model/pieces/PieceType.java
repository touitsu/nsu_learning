package org.chess.model.pieces;

public enum PieceType {
    OutOfBounds(0),
    Pawn(1),
    Bishop(2),
    Knight(3),
    Rook(4),
    Queen(5),
    King(6);

    private final int intValue;

    PieceType(int value) {
        this.intValue = value;
    }

    public int getValue() {
        return this.intValue;
    }

}
