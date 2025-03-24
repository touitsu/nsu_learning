package org.chess.model;

import org.chess.model.pieces.OutOfBounds;
import org.chess.model.pieces.Piece;

import java.util.ArrayList;

public final class Board {
    private ArrayList<ArrayList<Piece>> content;

    public Board() {
        this.content = new ArrayList<ArrayList<Piece>>();

        for (int i = 0; i < 8; i++) {
            this.content.add(new ArrayList<Piece>());
            for (int j = 0; j < 8; j++) {
                this.content.get(i).add(null);
            }
        }
    }

    public Piece getPieceAt(int x, int y) {
        try {
            return this.content.get(x).get(y);
        }
        catch (IndexOutOfBoundsException e) {
            return new OutOfBounds();
        }
    }

    public void setPiece(Piece piece, int x, int y) {
        try {
            this.content.get(x).set(y, piece);
        }
        catch (IndexOutOfBoundsException e) {
            throw new RuntimeException("Board index (" + x + ", " + y + ") is out of bounds while setting piece.");
        }

    }
}
