package org.chess.model;

import org.chess.connection.Message;
import org.chess.console.controller.ConsoleController;
import org.chess.console.exceptions.ConsoleException;
import org.chess.console.view.ConsoleOutput;
import org.chess.model.exceptions.ModelException;
import org.chess.player.Player;
import org.jetbrains.annotations.NotNull;

public final class GameStateController {

    private final Board board = new Board();
    private final ConsoleOutput console = new ConsoleOutput();
    private final Player player = new Player();
    private final ConsoleController controller = new ConsoleController(this.board, this, player);
    private boolean gameIsRunning = true;

    public void play() {

        this.player.setName(this.controller.readLine());
        this.console.printLine("Welcome, " + this.player.getName() + "!");

        while (this.gameIsRunning) {
            if (this.controller.isGameStarted()) {

                if (board.isGameEnded()) {
                    this.gameIsRunning = false;
                }
            }

            try {
                this.controller.handleInput();
            }
            catch (ConsoleException | ModelException e) {
                this.console.printLine(e.getMessage());
            }
        }

        if (board.isWhiteChecked()) {
            this.console.printLine("White are mated.\n");
        }

        else if (board.isBlackChecked()) {
            this.console.printLine("Black are mated.\n");
        }

        else {
            this.console.printLine("Game ended in pat.\n");
        }
    }

    public void printLine(@NotNull String string) {
        this.console.printLine(string);
    }

    public void draw() {
        this.console.draw(this.board);
    }
}
