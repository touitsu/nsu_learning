package org.chess;

import java.util.ArrayList;

public final class Board {
    private ArrayList<ArrayList<Piece>> content;

    public Board() {
        this.content = new ArrayList<ArrayList<Piece>>();

        for (int i = 0; i < 8; i++) {
            this.content.add(new ArrayList<Piece>());
        }
    }

    public Piece getPieceAt(int x, int y) {
        return this.content.get(x).get(y);
    }
}
