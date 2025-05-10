package org.chess.console.controller;

import org.chess.connection.Message;
import org.chess.console.exceptions.ConsoleException;
import org.chess.console.exceptions.InputFormatException;

import java.io.IOException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.chess.console.exceptions.UnknownCommandException;
import org.chess.controller.Controller;
import org.chess.model.Board;
import org.chess.model.GameStateController;
import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.pieces.Side;
import org.chess.player.Player;
import org.jetbrains.annotations.NotNull;

public final class ConsoleController extends Controller {

    private final Scanner scanner = new Scanner(System.in);
    private final Board board;
    private final GameStateController gameController;
    private static final Pattern movePattern = Pattern.compile("[a-h][1-8] ?[a-h][1-8].*");
    private static final Pattern hostPattern = Pattern.compile("h [0-9]{1,4}");
    private static final Pattern joinPattern = Pattern.compile("j [0-9.]{7,15} [0-9]{1,5}");
    private boolean isHost = false;

    public ConsoleController(@NotNull Board board, @NotNull GameStateController gameController, @NotNull Player player) {
        super(player);
        this.board = board;
        this.gameController = gameController;
    }

    private static String validateMove(@NotNull String str) throws InputFormatException {
        Matcher matcher;

        if (str.isEmpty()) {
            throw new InputFormatException("Input string is empty. (Correct input is \"[a-h][1-8] ?[a-h][1-8]\")\n");
        }

        matcher = movePattern.matcher(str);

        if (matcher.matches()) {
            return str.indexOf(' ') == -1 ? str.substring(0, 2) + ' ' + str.substring(2, 4) : str.substring(0, 5);
        }
        else {
            throw new InputFormatException("Input string doesn't match regex. (Correct input is \"[a-h][1-8] ?[a-h][1-8]\")\n");
        }
    }

    private static String validateCommand(@NotNull String str) throws InputFormatException {
        Matcher matcher;

        if (str.isEmpty()) {
            throw new InputFormatException("Input string is empty. (Correct input is \"h [0-9]{1,4}\" or \"j [0-9.]{7,15} [0-9]{1,4}\")\n");
        }

        matcher = hostPattern.matcher(str);

        if (matcher.matches()) {
            return str;
        }

        matcher = joinPattern.matcher(str);

        if (matcher.matches()) {
            return str;
        }

        if (str.charAt(0) == 'm') {
            return str;
        }

        else {
            throw new InputFormatException("Input string doesn't match regex. (Correct input is \"h [0-9]{1,4}\" or \"j [0-9.]{7,15} [0-9]{1,4}\")\n");
        }
    }

    @Override
    public void handleInput() throws MoveUnavailableException, UnknownCommandException {
        String command, input;

        command = null;
        input = this.scanner.nextLine();

        if (this.gameStarted) {
            try {
                command = validateCommand(input);

                if (command.charAt(0) == 'm') {
                    sendMessage(command.substring(2));
                }
            }
            catch (InputFormatException ignore) {
                try {
                    command = validateMove(input);

                    board.move(command);
                }
                catch (InputFormatException e) {
                    throw new MoveUnavailableException(e.getMessage());
                }
            }
        }
        else {
            try {
                command = validateCommand(input);

                if (command.charAt(0) == 'h') {
                    host(Integer.parseInt(command.substring(2)));
                }
                else if (command.charAt(0) == 'j') {
                    join(command.substring(2, command.indexOf(' ', 2)), Integer.parseInt(command.substring(command.indexOf(' ', 2) + 1)));
                }
            }
            catch (InputFormatException e) {
                throw new UnknownCommandException("Command " + input.substring(0, input.indexOf(' ') == -1? input.length() : input.indexOf(' ')) + " is unknown.\n");
            }
        }
    }

    @Override
    public void handleDisconnection() {

    }

    @Override
    public void handleOpponentMove(String input) throws MoveUnavailableException {
        String move;

        try {
            move = validateMove(input);
        }
        catch (InputFormatException e) {
            throw new MoveUnavailableException(e.getMessage());
        }

        board.move(move);
    }

    @Override
    public void host(int port) {
        this.side = Side.WHITE;

        try {
            this.gameController.printLine("Waiting for opponent...\n");
            this.networkManager.host(port);
            this.isHost = true;
            this.gameController.printLine("Successfully hosted game on port " + port + ".\n");
            startGame();
        }
        catch(IOException e) {
            this.gameController.printLine("Failed to host game.\n" + e.getMessage());
        }
    }

    @Override
    public void join(@NotNull String host, int port) {
        this.side = Side.BLACK;

        try {
            this.networkManager.join(host, port);
            this.gameController.printLine("Successfully joined game.\n");
            startGame();
        }
        catch(IOException e) {
            this.gameController.printLine("Failed to join game.\nReason: " + e.getMessage());
        }
    }

    @Override
    public void sendMessage(@NotNull String message) {
        try {
            this.networkManager.sendMessage(message);
        }
        catch (IOException e) {
            this.gameController.printLine(e.getMessage());
        }
        catch (NullPointerException e) {
            this.gameController.printLine("Failed to send message, because there is nowhere to send it.\n");
        }
    }

    @Override
    public void receiveMessage(@NotNull Message message) {
        this.gameController.printLine(message.getSender().getName() + ": " + message.getData());
    }

    public void startGame() throws IOException{
        this.networkManager.sendStartGame();
    }

    @Override
    public void receiveStartGame(@NotNull Message message) {
        this.gameStarted = true;
        if (isHost) {
            this.gameController.printLine("Player " + message.getSender().getName() + " joined your game.\n");
            this.gameController.draw();
        }
        else {
            this.gameController.printLine("You've joined " + message.getSender().getName() + "'s game.\n");
            this.gameController.draw();
        }
    }

    public String readLine() {
        return this.scanner.nextLine();
    }

    public boolean isGameStarted() {
        return this.gameStarted;
    }
}
