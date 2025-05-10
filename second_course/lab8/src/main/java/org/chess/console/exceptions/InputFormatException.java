package org.chess.console.exceptions;

import org.jetbrains.annotations.NotNull;

public final class InputFormatException extends ConsoleException {

    public InputFormatException(@NotNull String msg) {
        super(msg);
    }
}
