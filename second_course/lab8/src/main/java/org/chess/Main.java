package org.chess;

import org.chess.consoleController.ConsoleController;
import org.chess.consoleExceptions.InputFormatException;
import org.chess.consoleView.ConsoleOutput;
import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.chess.model.GameController;
import org.chess.model.pieces.*;

public class Main {

    public static void main(String[] argv) {
        GameController game = new GameController();

        game.play();
    }

}
