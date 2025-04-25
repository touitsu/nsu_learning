package org.chess.model;

import org.chess.consoleController.ConsoleController;
import org.chess.consoleExceptions.InputFormatException;
import org.chess.consoleView.ConsoleOutput;
import org.chess.model.exceptions.GameEndedException;
import org.chess.model.exceptions.MoveUnavailableException;

public final class GameController {

    private final Board board = new Board();
    private final ConsoleOutput console = new ConsoleOutput();
    private final ConsoleController controller = new ConsoleController(this.board);
    private boolean gameIsRunning = true;

    public void play() {
        this.console.draw(this.board);

        while (gameIsRunning) {
            try {
                this.controller.handleInput();
            }
            catch (MoveUnavailableException | InputFormatException e) {
                this.console.printStatus(e.getMessage());
            }
            catch (GameEndedException e) {
                this.console.printStatus(e.getMessage());
                this.gameIsRunning = false;
            }

            this.console.draw(this.board);
        }
    }

}
