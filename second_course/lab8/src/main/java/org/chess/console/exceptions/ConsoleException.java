package org.chess.console.exceptions;

import org.jetbrains.annotations.NotNull;

public class ConsoleException extends Exception {

    public ConsoleException(@NotNull String msg) {
        super(msg);
    }

}
