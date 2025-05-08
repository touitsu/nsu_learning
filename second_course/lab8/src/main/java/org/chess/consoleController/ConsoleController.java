package org.chess.consoleController;

import org.chess.consoleExceptions.InputFormatException;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.chess.controller.Controller;
import org.chess.model.Board;
import org.chess.model.exceptions.MoveUnavailableException;
import org.jetbrains.annotations.NotNull;

public final class ConsoleController extends Controller {

    private static final Scanner scanner = new Scanner(System.in);
    private final Board board;
    private static final Pattern pattern = Pattern.compile("[a-h][1-8] ?[a-h][1-8].*");

    public ConsoleController(@NotNull Board board) {
        this.board = board;
    }

    private static String validateString(@NotNull String str) throws InputFormatException {
        Matcher matcher;

        if (str.isEmpty()) {
            throw new InputFormatException("Input string is empty. (Correct input is \"[a-h][1-8] ?[a-h][1-8]\")\n");
        }

        matcher = pattern.matcher(str);

        if (matcher.matches()) {
            return str.indexOf(' ') == -1 ? str.substring(0, 2) + ' ' + str.substring(2, 4) : str.substring(0, 5);  // Returns in format "e2 e4"
        }
        else {
            throw new InputFormatException("Input string doesn't match regex. (Correct input is \"[a-h][1-8] ?[a-h][1-8]\")\n");
        }
    }

    @Override
    public void handleInput() throws MoveUnavailableException {
        String move;

        try {
            move = validateString(scanner.nextLine());
        }
        catch (InputFormatException e) {
            throw new MoveUnavailableException(e.getMessage());
        }

        board.move(move);
    }

}
