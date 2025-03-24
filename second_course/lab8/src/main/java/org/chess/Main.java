package org.chess;

import org.chess.consoleView.ConsoleOutput;
import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.chess.model.pieces.*;

public class Main {

    public static void main(String[] argv) {
        Board board = new Board();
        for (int i = 0; i < 8; i++) {
            board.setPiece(new Pawn(Side.WHITE), i, 6);
            board.setPiece(new Pawn(Side.BLACK), i, 1);
        }

        board.setPiece(new Rook(Side.WHITE), 0, 7);
        board.setPiece(new Rook(Side.WHITE), 7, 7);
        board.setPiece(new Rook(Side.BLACK), 0, 0);
        board.setPiece(new Rook(Side.BLACK), 7, 0);

        board.setPiece(new Knight(Side.WHITE), 1, 7);
        board.setPiece(new Knight(Side.WHITE), 6, 7);
        board.setPiece(new Knight(Side.BLACK), 1, 0);
        board.setPiece(new Knight(Side.BLACK), 6, 0);

        board.setPiece(new Bishop(Side.WHITE), 2, 7);
        board.setPiece(new Bishop(Side.WHITE), 5, 7);
        board.setPiece(new Bishop(Side.BLACK), 2, 0);
        board.setPiece(new Bishop(Side.BLACK), 5, 0);

        board.setPiece(new Queen(Side.WHITE), 3, 7);
        board.setPiece(new King(Side.WHITE), 4, 7);
        board.setPiece(new Queen(Side.BLACK), 3, 0);
        board.setPiece(new King(Side.BLACK), 4, 0);

        ConsoleOutput console = new ConsoleOutput();

        console.draw(board);

        System.out.println(board.getPieceAt(1, 7).calculateAvailableMoves(board, new Coordinates(1, 7)));
    }

}
