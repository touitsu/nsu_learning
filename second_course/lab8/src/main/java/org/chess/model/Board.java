package org.chess.model;

import org.chess.model.exceptions.GameEndedException;
import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.pieces.*;
import org.jetbrains.annotations.NotNull;

import java.util.HashMap;

public final class Board {
    private HashMap<Coordinates, Piece> pieces;
    private Side currentMove;
    private final Piece blackKing = new King(Side.BLACK, new Coordinates(4, 7));
    private final Piece whiteKing = new King(Side.WHITE, new Coordinates(4, 0));
    private boolean isBlackChecked;
    private boolean isWhiteChecked;

    public Board() {
        this.currentMove = Side.WHITE;

        this.pieces = new HashMap<>();

        for (int i = 0; i < 8; i++) {
            this.setPiece(new Pawn(Side.WHITE, new Coordinates(i, 1)));
            this.setPiece(new Pawn(Side.BLACK, new Coordinates(i, 6)));
        }

        this.setPiece(new Rook(Side.WHITE, new Coordinates(0, 0)));
        this.setPiece(new Rook(Side.WHITE, new Coordinates(7, 0)));
        this.setPiece(new Rook(Side.BLACK, new Coordinates(0, 7)));
        this.setPiece(new Rook(Side.BLACK, new Coordinates(7, 7)));

        this.setPiece(new Knight(Side.WHITE, new Coordinates(1, 0)));
        this.setPiece(new Knight(Side.WHITE, new Coordinates(6, 0)));
        this.setPiece(new Knight(Side.BLACK, new Coordinates(1, 7)));
        this.setPiece(new Knight(Side.BLACK, new Coordinates(6, 7)));

        this.setPiece(new Bishop(Side.WHITE, new Coordinates(2, 0)));
        this.setPiece(new Bishop(Side.WHITE, new Coordinates(5, 0)));
        this.setPiece(new Bishop(Side.BLACK, new Coordinates(2, 7)));
        this.setPiece(new Bishop(Side.BLACK, new Coordinates(5, 7)));

        this.setPiece(new Queen(Side.WHITE, new Coordinates(3, 0)));
        this.setPiece(this.whiteKing);
        this.setPiece(new Queen(Side.BLACK, new Coordinates(3, 7)));
        this.setPiece(this.blackKing);
    }


    @SuppressWarnings("unchecked")
    private Board copy() {
        final Board res;

        res = new Board();

        res.pieces = (HashMap<Coordinates, Piece>) this.pieces.clone();

        return res;
    }


    public Piece getPieceAt(int x, int y) {
        return pieces.get(new Coordinates(x, y));
    }


    public Piece getPieceAt(@NotNull Coordinates coordinates) {
        return pieces.get(coordinates);
    }

    private void setPiece(@NotNull Piece piece) {
        if (piece.getCoordinates().x() < 0 && piece.getCoordinates().x() > 8 && piece.getCoordinates().y() < 0 && piece.getCoordinates().y() > 8) {
            throw new RuntimeException("Board index " + piece.getCoordinates() + " is out of bounds while setting piece.");
        }

        pieces.put(piece.getCoordinates(), piece);
    }


    private boolean doesAnyPieceAttack(@NotNull Coordinates coordinates, @NotNull Side side) {

        for (Piece piece : this.pieces.values()) {
            if (piece.getSide() == side.opposite()) {
                if (piece.calculateAvailableMoves(this, piece.getCoordinates()).contains(coordinates)) {
                    return true;
                }
            }
        }

        return false;
    }


    private boolean moveEliminatesCheck(@NotNull Coordinates start, @NotNull Coordinates end, @NotNull Piece king) {
        final Piece pieceAtStart;
        final Piece pieceAtEnd;
        final boolean res;
        final Board newBoard;

        newBoard = this.copy();

        pieceAtStart = newBoard.getPieceAt(start);
        pieceAtEnd = newBoard.getPieceAt(end);

        if (pieceAtEnd != null) {
            newBoard.pieces.remove(end);
        }

        pieceAtStart.move(end);
        newBoard.pieces.remove(start);
        newBoard.setPiece(pieceAtStart);

        res = newBoard.doesAnyPieceAttack(king.getCoordinates(), king.getSide());

        pieceAtStart.move(start);

        return !res;
    }


    private boolean isMoveAvailable(@NotNull Coordinates start, @NotNull Coordinates end, @NotNull Side side) throws MoveUnavailableException {
        final Piece piece;

        piece = getPieceAt(start);

        //nothing to move
        if (piece == null) {
            throw new MoveUnavailableException("There is no piece at " + start.toChessNotation() + ".\n");
        }

        //moving wrong side
        if (piece.getSide() != side) {
            throw new MoveUnavailableException("Pieces with side " + piece.getSide() + " can't move while it is " + side + " turn.\n");
        }

        if (!piece.calculateAvailableMoves(this, start).contains(end)) {
            throw new MoveUnavailableException("Move " + end.toChessNotation() + " is not available for piece at " + start.toChessNotation() + ".\n");
        }

        //trying to capture piece of the same side
        if (getPieceAt(end) != null) {
            if (getPieceAt(end).getSide() == piece.getSide()) {
                throw new MoveUnavailableException(piece.getSide() + " piece can't capture " + piece.getSide() + " piece.\n");
            }
        }

        //trying to do a capture move with a pawn with nothing to capture
        if (piece.getType() == PieceType.Pawn && ((end.equals(new Coordinates(start.x() + 1, start.y() + piece.getSide().getValue())) && getPieceAt(end) == null) ||
                (end.equals(new Coordinates(start.x() - 1, start.y() + piece.getSide().getValue())) && getPieceAt(end) == null))) {
            throw new MoveUnavailableException("Pawn can't move that way unless there is something to capture.\n");
        }

        //trying to get urself checked
        if (piece.getType() == PieceType.King) {
            if (doesAnyPieceAttack(end, side)) {
                throw new MoveUnavailableException("King can't move into check.\n");
            }
        }

        //Checks must be eliminated
        if (this.isWhiteChecked) {
            if (!moveEliminatesCheck(start, end, this.whiteKing)) {
                throw new MoveUnavailableException("Your move must eliminate check.\n");
            }
        }

        if (this.isBlackChecked) {
            if (!moveEliminatesCheck(start, end, this.blackKing)) {
                throw new MoveUnavailableException("Your move must eliminate check.\n");
            }
        }

        return true;
    }


    private boolean hasAnyMoves(@NotNull Piece king) {

        for (Piece piece : this.pieces.values()) {
            if (piece.getSide() == king.getSide()) {
                for (Coordinates move : piece.calculateAvailableMoves(this, piece.getCoordinates())) {
                    try {
                        if (isMoveAvailable(piece.getCoordinates(), move, king.getSide())) {
                            System.out.println(piece + " " + piece.getCoordinates() + " " + move);
                            return true;
                        }
                    }
                    catch (MoveUnavailableException ignore) {

                    }
                }
            }
        }

        return false;
    }


    public void move(@NotNull String move) throws MoveUnavailableException, GameEndedException {
        final Coordinates start, end;
        final Piece piece;

        start = new Coordinates(move.charAt(0) - 'a', Character.digit(move.charAt(1), 10) - 1);
        end = new Coordinates(move.charAt(3) - 'a', Character.digit(move.charAt(4), 10) - 1);

        piece = getPieceAt(start);

        if (isMoveAvailable(start, end, this.currentMove)) {

            System.out.println(piece.getType() + " " + piece.getSide());
            System.out.println(piece.calculateAvailableMoves(this, start));

            if (getPieceAt(end) != null) {
                this.pieces.remove(end);
            }

            piece.move(end);
            this.pieces.remove(start);
            setPiece(piece);

            if (this.currentMove == Side.BLACK) {
                if (this.isBlackChecked) {
                    this.isBlackChecked = false;
                }

                if (doesAnyPieceAttack(this.whiteKing.getCoordinates(), this.whiteKing.getSide())) {
                    this.isWhiteChecked = true;

                    if (!hasAnyMoves(this.whiteKing)) {
                        throw new GameEndedException("White are checkmated!\n");
                    }
                }
            }

            else {
                if (this.isWhiteChecked) {
                    this.isWhiteChecked = false;
                }

                if (doesAnyPieceAttack(this.blackKing.getCoordinates(), this.blackKing.getSide())) {
                    this.isBlackChecked = true;

                    if (!hasAnyMoves(this.blackKing)) {
                        throw new GameEndedException("Black are checkmated!\n");
                    }
                }
            }

            this.currentMove = this.currentMove == Side.BLACK ? Side.WHITE : Side.BLACK;
        }
    }
}
