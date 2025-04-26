package org.chess.model.exceptions;

public final class GameEndedException extends Exception {
    public GameEndedException(String msg) {
        super(msg);
    }
}
