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
    private volatile boolean gameIsRunning = true;
    private final Thread input = new Thread(this::waitInput);

    public void play() {

        this.player.setName(this.controller.readLine());
        this.console.printLine("Welcome, " + this.player.getName() + "!");

        this.input.start();

        while (this.gameIsRunning) {
            if (!this.controller.isGameRunning()) {
                this.gameIsRunning = false;
                this.input.interrupt();
            }
        }
    }

    private void waitInput() {
        while(this.gameIsRunning) {
            try {
                this.controller.handleInput();
            }
            catch (ConsoleException | ModelException e) {
                if (this.gameIsRunning) {
                    this.console.printLine(e.getMessage());
                }
            }
        }
    }


    public void printLine(@NotNull String string) {
        this.console.printLine(string);
    }

    public void draw() {
        this.console.draw(this.board);
    }
}
