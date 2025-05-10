package org.chess.model;

import org.chess.connection.NetworkManager;
import org.chess.consoleController.ConsoleController;
import org.chess.consoleView.ConsoleOutput;
import org.chess.model.exceptions.MoveUnavailableException;

public final class GameStateController {

    private final Board board = new Board();
    private final ConsoleOutput console = new ConsoleOutput();
    private final ConsoleController controller = new ConsoleController(this.board);
    private boolean gameIsRunning = true;

    public void play() {
        this.console.draw(this.board);

        while (this.gameIsRunning) {
            try {
                this.controller.handleInput();
            }
            catch (MoveUnavailableException e) {
                this.console.printStatus(e.getMessage());
            }

            //this.console.draw(this.board);

            if (board.isGameEnded()) {
                this.gameIsRunning = false;
            }
        }

        if (board.isWhiteChecked()) {
            this.console.printStatus("White are mated.\n");
        }

        else if (board.isBlackChecked()) {
            this.console.printStatus("Black are mated.\n");
        }

        else {
            this.console.printStatus("Game ended in pat.\n");
        }
    }
}
