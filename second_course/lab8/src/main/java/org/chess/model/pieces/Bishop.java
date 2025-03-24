package org.chess.model.pieces;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public final class Bishop extends Piece {

    public Bishop(Side side) {
        super(side, PieceType.Bishop);
    }

    @Override
    public ArrayList<Coordinates> calculateAvailableMoves(@NotNull Board currentBoard, @NotNull Coordinates position) {
        ArrayList<Coordinates> res;

        res = new ArrayList<Coordinates>();

        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1; i++) {
            if (isMoveAvailable(currentBoard, new Coordinates(position.x() + i, position.y() + i))) {
                res.add(new Coordinates(position.x() + i, position.y() + i));
                if (currentBoard.getPieceAt(position.x() + i, position.y() + i) != null) {
                    breakFlag = 1;
                }
            }
            else {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1; i++) {
            if (isMoveAvailable(currentBoard, new Coordinates(position.x() + i, position.y() - i))) {
                res.add(new Coordinates(position.x() + i, position.y() - i));
                if (currentBoard.getPieceAt(position.x() + i, position.y() - i) != null) {
                    breakFlag = 1;
                }
            }
            else {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag == 0; i++) {
            if (isMoveAvailable(currentBoard, new Coordinates(position.x() - i, position.y() + i))) {
                res.add(new Coordinates(position.x() - i, position.y() + i));
                if (currentBoard.getPieceAt(position.x() - i, position.y() + i) != null) {
                    breakFlag = 1;
                }
            }
            else {
                breakFlag = 1;
            }
        }
        for (int i = 1, breakFlag = 0; i < 8 && breakFlag != 1; i++) {
            if (isMoveAvailable(currentBoard, new Coordinates(position.x() - i, position.y() - i))) {
                res.add(new Coordinates(position.x() - i, position.y() - i));
                if (currentBoard.getPieceAt(position.x() - i, position.y() - i) != null) {
                    breakFlag = 1;
                }
            }
            else {
                breakFlag = 1;
            }
        }

        return res;
    }

}
