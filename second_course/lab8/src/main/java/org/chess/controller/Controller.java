package org.chess.controller;

import org.chess.connection.Message;
import org.chess.connection.NetworkManager;
import org.chess.console.exceptions.UnknownCommandException;
import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.pieces.Side;
import org.chess.player.Player;
import org.jetbrains.annotations.NotNull;

public abstract class Controller {

    protected final NetworkManager networkManager;
    protected Side side;
    protected final Player player;
    protected volatile boolean gameStarted;

    public Controller(@NotNull Player player) {
        this.networkManager = new NetworkManager(this);
        this.player = player;
    }

    abstract public void handleInput() throws MoveUnavailableException, UnknownCommandException;

    abstract public void handleDisconnection();

    abstract public void handleOpponentMove(String move) throws MoveUnavailableException;

    abstract public void host(int port);

    abstract public void sendMessage(@NotNull String message);

    abstract public void receiveMessage(@NotNull Message message);

    abstract public void join(String host, int port);

    abstract public void receiveStartGame(@NotNull Message message);

    public Player getPlayer() {
        return this.player;
    }
}
