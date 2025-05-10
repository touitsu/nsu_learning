package org.chess.consoleController;

import org.chess.connection.NetworkManager;
import org.chess.consoleExceptions.InputFormatException;

import java.io.IOException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.chess.controller.Controller;
import org.chess.model.Board;
import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.pieces.Side;
import org.jetbrains.annotations.NotNull;

public final class ConsoleController extends Controller {

    private final Scanner scanner = new Scanner(System.in);
    private final Board board;
    private static final Pattern movePattern = Pattern.compile("[a-h][1-8] ?[a-h][1-8].*");
    private static final Pattern hostPattern = Pattern.compile("h [0-9]{1,4}");
    private static final Pattern joinPattern = Pattern.compile("j [0-9.]{7,15} [0-9]{1,4}");
    private boolean gameStarted;

    public ConsoleController(@NotNull Board board) {
        super();
        this.board = board;
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
    public void handleInput() throws MoveUnavailableException {
        String command;

        command = null;

        try {
            command = validateCommand(this.scanner.nextLine());

            if (command.charAt(0) == 'h') {
                host(Integer.parseInt(command.substring(2)));
            }
            else if (command.charAt(0) == 'j') {
                join(command.substring(2, command.indexOf(' ', 2)), Integer.parseInt(command.substring(command.indexOf(' ', 2) + 1)));
            }
            else {
                message(command.substring(2));
            }
        }
        catch (InputFormatException e) {

        }

        if (this.gameStarted) {
            try {
                command = validateMove(this.scanner.nextLine());
            }
            catch (InputFormatException e) {
                //throw new MoveUnavailableException(e.getMessage());
            }

            if (command != null) {
                board.move(command);
            }
        }

    }

    @Override
    public void handleDisconnection() {

    }

    @Override
    public void handleOpponentMove(String move) throws MoveUnavailableException {

        try {
            move = validateMove(scanner.nextLine());
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
            System.out.println("Waiting for opponent...\n");
            this.networkManager.host(port);
        }
        catch(IOException e) {
            System.out.println("Failed to host game.\n" + e.getMessage());
        }

        System.out.println("Successfully hosted game on port " + port + ".\n");
    }

    @Override
    public void join(@NotNull String host, int port) {
        this.side = Side.BLACK;

        try {
            this.networkManager.join(host, port);
        }
        catch(IOException e) {
            System.out.println("Failed to join game.\n" + e.getMessage());
        }

        System.out.println("Successfully joined game.\n");
    }

    public void message(@NotNull String message) {
        try {
            this.networkManager.sendMessage(message);
        }
        catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }

}
