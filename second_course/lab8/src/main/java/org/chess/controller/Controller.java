package org.chess.controller;

import org.chess.connection.Message;
import org.chess.connection.NetworkManager;
import org.chess.console.exceptions.NoInputException;
import org.chess.console.exceptions.UnknownCommandException;
import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.pieces.Side;
import org.chess.player.Player;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;

public abstract class Controller {

    protected final NetworkManager networkManager;
    protected Side side;
    protected final Player player;
    protected boolean gameStarted;
    protected volatile boolean gameRunning = true;

    public Controller(@NotNull Player player) {
        this.networkManager = new NetworkManager(this);
        this.player = player;
    }

    abstract public void handleInput() throws MoveUnavailableException, UnknownCommandException, NoInputException;

    abstract public void handleDisconnection();

    abstract public void sendMove(@NotNull String move);

    abstract public void handleOpponentMove(@NotNull Message message);

    abstract public void host(int port);

    abstract public void sendMessage(@NotNull String message);

    abstract public void handleMessage(@NotNull Message message);

    abstract public void join(@NotNull String host, int port);

    abstract public void sendStartGame() throws IOException;

    abstract public void handleStartGame(@NotNull Message message);

    abstract public void sendEndGame(@NotNull String reason) throws IOException;

    abstract public void handleEndGame(@NotNull Message message);

    abstract public void sendContinueGame() throws IOException;

    abstract public void handleContinueGame();

    abstract public void sendSync() throws IOException;

    abstract public void handleSync(@NotNull Message message);

    public Player getPlayer() {
        return this.player;
    }

    abstract public void endGame();

    abstract public void handleReconnection() throws IOException;

    abstract public void writeMessage(String str);
}
