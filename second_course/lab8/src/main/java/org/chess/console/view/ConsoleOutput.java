package org.chess.console.view;

import org.chess.model.*;
import org.chess.model.pieces.*;
import org.jetbrains.annotations.NotNull;
import org.jetbrains.annotations.Nullable;

public final class ConsoleOutput {

    public void printLine(@NotNull String str) {
        System.out.println(str);
    }

    public ConsoleOutput() {
        System.out.println("Welcome to multiplayer chess.\nTo host a game enter \"h {port}\".\nTo join a game enter \"j {host ip} {host port}\"." +
                "\nTo talk with your opponent use \"m {your message}\".\nTo make a move just input starting coordinates and end coordinates (e.g. e2 e4).\n\n\nPlayer name: ");
    }

    private static String determinePieceOutput(@Nullable Piece piece) {
        String symbols = " pbkrQK";
        String res;

        res = "";

        if (piece == null) {
            res += symbols.charAt(0);
            res += symbols.charAt(0);
        }
        else {
            if (piece.getSide() == Side.BLACK) {
                res = "\u001B[35m";
            }
            else {
                res = "\u001B[36m";
            }

            res += symbols.charAt(piece.getType().getValue());

            res += "\u001B[0m ";
        }

        return res;
    }

    public void draw(@NotNull Board board) {

        System.out.print("  ");

        for (int y = 0; y < 8; y++) {
            System.out.print((char)(y + 'a') + " ");
        }

        System.out.println();

        for (int y = 8; y > 0; y--) {
            System.out.print(y);
            System.out.print(" ");
            for (int x = 1; x < 9; x++) {

                System.out.print(determinePieceOutput(board.getPieceAt(x-1, y-1)));
            }
            System.out.println();
        }

        System.out.println();
    }
}
