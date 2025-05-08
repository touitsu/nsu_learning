package org.chess.controller;

import org.chess.model.exceptions.MoveUnavailableException;

public abstract class Controller {

    abstract public void handleInput() throws MoveUnavailableException;

    abstract public void handleDisconnection();
}
