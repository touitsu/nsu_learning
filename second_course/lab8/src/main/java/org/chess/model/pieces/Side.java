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


    public Side opposite() {
        if (this.intValue == 1) {
            return BLACK;
        }

        return WHITE;
    }


    @Override
    public String toString() {
        if (this.intValue == 1) {
            return "White";
        }

        return "Black";
    }
}
