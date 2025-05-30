package org.chess.connection;

import org.chess.player.Player;
import org.jetbrains.annotations.NotNull;

import java.io.Serializable;

public final class Message implements Serializable {
    private final MessageType type;
    private final Player sender;
    private final Object data;

    public Message(@NotNull MessageType type, @NotNull Player player, Object data) {
        this.type = type;
        this.sender = player;
        this.data = data;
    }

    public MessageType getType() {
        return this.type;
    }

    public Object getData() {
        return this.data;
    }

    public Player getSender() {
        return this.sender;
    }
}
