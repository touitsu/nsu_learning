package org.chess.connection;

import org.jetbrains.annotations.NotNull;

import java.io.Serializable;

public final class Message implements Serializable {
    private final MessageType type;
    private final Object data;

    public Message(@NotNull MessageType type, @NotNull Object data) {
        this.type = type;
        this.data = data;
    }

    public MessageType getType() {
        return this.type;
    }

    public Object getData() {
        return this.data;
    }
}
