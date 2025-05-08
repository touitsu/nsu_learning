package org.chess.model;

import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.exceptions.RepetitionException;
import org.chess.model.pieces.*;
import org.jetbrains.annotations.NotNull;

import java.util.HashMap;

public final class Board {
    private HashMap<Coordinates, Piece> pieces;
    private Side currentMove;
    private final Piece blackKing = new King(Side.BLACK, new Coordinates(4, 7));
    private final Piece whiteKing = new King(Side.WHITE, new Coordinates(4, 0));
    private MoveHistory history = new MoveHistory();
    private boolean blackChecked = false;
    private boolean whiteChecked = false;
    private boolean gameEnded = false;
    private boolean pat = false;

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
                if (piece.getMoves(this, piece.getCoordinates()).contains(coordinates)) {
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

        if (!piece.getMoves(this, start).contains(end)) {
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

            //castling under attack
            if (start.x() - end.x() < -1) {
                if (doesAnyPieceAttack(new Coordinates(start.x() + 1, start.y()), side) || doesAnyPieceAttack(new Coordinates(start.x() + 2, start.y()), side)) {
                    throw new MoveUnavailableException("Can't castle with pieces attacking castling positions.\n");
                }
            }

            if (start.x() - end.x() > 1) {
                if (doesAnyPieceAttack(new Coordinates(start.x() - 1, start.y()), side) || doesAnyPieceAttack(new Coordinates(start.x() - 2, start.y()), side)) {
                    throw new MoveUnavailableException("Can't castle with pieces attacking castling positions.\n");
                }
            }
        }

        //Checks must be eliminated
        if (this.whiteChecked) {
            if (!moveEliminatesCheck(start, end, this.whiteKing)) {
                throw new MoveUnavailableException("Your move must eliminate check.\n");
            }
        }

        if (this.blackChecked) {
            if (!moveEliminatesCheck(start, end, this.blackKing)) {
                throw new MoveUnavailableException("Your move must eliminate check.\n");
            }
        }

        return true;
    }


    private boolean pieceHasAnyMoves(@NotNull Piece piece) {

        for (Piece otherPiece : this.pieces.values()) {
            if (otherPiece.getSide() == piece.getSide()) {
                for (Coordinates move : otherPiece.getMoves(this, otherPiece.getCoordinates())) {
                    try {
                        if (isMoveAvailable(otherPiece.getCoordinates(), move, piece.getSide())) {
                            System.out.println(otherPiece + " " + otherPiece.getCoordinates() + " " + move);
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


    private boolean sideHasAnyMoves(@NotNull Side side) {

        for (Piece piece : this.pieces.values()) {
            if (piece.getSide() == side) {
                for (Coordinates move : piece.getMoves(this, piece.getCoordinates())) {
                    try {
                        if (isMoveAvailable(piece.getCoordinates(), move, piece.getSide())) {
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


    public boolean isGameEnded() {
        return this.gameEnded;
    }


    public boolean isWhiteChecked() {
        return this.whiteChecked;
    }


    public boolean isBlackChecked() {
        return this.blackChecked;
    }


    public boolean isPat() {
        return this.pat;
    }


    public void move(@NotNull String move) throws MoveUnavailableException {
        final Coordinates start, end;
        Piece piece;
        int knightsCnt;
        Side knightSide;

        start = new Coordinates(move.charAt(0) - 'a', Character.digit(move.charAt(1), 10) - 1);
        end = new Coordinates(move.charAt(3) - 'a', Character.digit(move.charAt(4), 10) - 1);

        piece = getPieceAt(start);

        if (isMoveAvailable(start, end, this.currentMove)) {

            System.out.println(piece.getType() + " " + piece.getSide());
            System.out.println(piece.getMoves(this, start));

            if (getPieceAt(end) != null) {
                this.pieces.remove(end);
            }

            piece.move(end);
            this.pieces.remove(start);
            setPiece(piece);

            //castling
            if (piece.getType() == PieceType.King) {
                if (start.x() - end.x() < -1) {
                    piece = getPieceAt(new Coordinates(7, piece.getCoordinates().y()));
                    this.pieces.remove(piece.getCoordinates());
                    piece.move(new Coordinates(5, piece.getCoordinates().y()));
                    setPiece(piece);
                }

                if (start.x() - end.x() > 1) {
                    piece = getPieceAt(new Coordinates(0, piece.getCoordinates().y()));
                    this.pieces.remove(piece.getCoordinates());
                    piece.move(new Coordinates(3, piece.getCoordinates().y()));
                    setPiece(piece);
                }
            }

            //promoting
            if (piece.getType() == PieceType.Pawn && (piece.getCoordinates().y() == 0 || piece.getCoordinates().y() == 7)) {
                this.pieces.remove(piece.getCoordinates());
                piece = new Queen(piece.getSide(), piece.getCoordinates());
                setPiece(piece);
            }

            if (this.currentMove == Side.BLACK) {
                if (this.blackChecked) {
                    this.blackChecked = false;
                }

                if (doesAnyPieceAttack(this.whiteKing.getCoordinates(), this.whiteKing.getSide())) {
                    this.whiteChecked = true;

                    if (!pieceHasAnyMoves(this.whiteKing)) {
                        this.gameEnded = true;
                    }
                }

                if (!sideHasAnyMoves(Side.WHITE)) {
                    this.pat = true;
                }
            }

            else {
                if (this.whiteChecked) {
                    this.whiteChecked = false;
                }

                if (doesAnyPieceAttack(this.blackKing.getCoordinates(), this.blackKing.getSide())) {
                    this.blackChecked = true;

                    if (!pieceHasAnyMoves(this.blackKing)) {
                        this.gameEnded = true;
                    }
                }

                if (!sideHasAnyMoves(Side.BLACK)) {
                    this.pat = true;
                }
            }

            //pat with only kings left
            if (pieces.values().size() == 2) {
                this.pat = true;
            }

            //pat with only 1 knight from each side left
            knightsCnt = 0;
            knightSide = null;

            if (pieces.values().size() == 3 || pieces.values().size() == 4) {
                for (Piece leftPiece : this.pieces.values()) {
                    if (leftPiece.getType() == PieceType.Knight && knightSide != leftPiece.getSide()) {
                        if (knightSide == null) {
                            knightSide = leftPiece.getSide();
                        }
                        knightsCnt++;
                    }
                }

                if ((pieces.values().size() == 3 && knightsCnt == 1) || (pieces.values().size() == 4 && knightsCnt == 2)) {
                    this.pat = true;
                }
            }

            try {
                this.history.add(move);
            }
            catch (RepetitionException e) {
                this.pat = true;
            }

            if (this.pat) {
                this.gameEnded = true;
            }

            this.currentMove = this.currentMove == Side.BLACK ? Side.WHITE : Side.BLACK;
        }
    }
}
