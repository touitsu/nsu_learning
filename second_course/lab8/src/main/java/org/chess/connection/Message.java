package org.chess.connection;

import java.io.Serializable;

public final class Message implements Serializable {
    private final MessageType type;
    private final Object data;

    public Message(MessageType type, Object data) {
        this.type = type;
        this.data = data;
    }
}
