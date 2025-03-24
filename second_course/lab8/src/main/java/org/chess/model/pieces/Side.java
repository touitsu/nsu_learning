package org.chess.model.pieces;

public enum Side {
    WHITE(1),
    BLACK(-1);

    private final int intValue;

    Side(int value) {
        this.intValue = value;
    }

    public int getValue() {
        return this.intValue;
    }
}
