package org.chess.controller;

import org.chess.connection.NetworkManager;
import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.pieces.Side;

public abstract class Controller {

    protected final NetworkManager networkManager;
    protected Side side;

    public Controller() {
        this.networkManager = new NetworkManager(this);
    }

    abstract public void handleInput() throws MoveUnavailableException;

    abstract public void handleDisconnection();

    abstract public void handleOpponentMove(String move) throws MoveUnavailableException;

    abstract public void host(int port);

    abstract public void join(String host, int port);
}
