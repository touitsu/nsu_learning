package org.chess.gui.view;

import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.chess.model.pieces.Piece;
import org.chess.model.pieces.Side;
import org.chess.gui.controller.GuiController;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.awt.*;
import java.util.HashMap;

public class GuiView extends JFrame {
    private final GuiController controller;
    private final Board board;
    private JButton[][] squares;
    private JTextArea messageArea;
    private JTextField hostField;
    private JTextField portField;
    private JTextField messageField;
    private JButton hostButton;
    private JButton joinButton;
    private JButton sendMessageButton;
    private JLabel statusLabel;
    private JPanel boardPanel;
    private JPanel controlPanel;

    private Coordinates selectedSquare = null;
    private static final Color LIGHT_SQUARE = new Color(240, 217, 181);
    private static final Color DARK_SQUARE = new Color(181, 136, 99);
    private static final Color SELECTED_COLOR = new Color(255, 255, 0, 128);
    private static final Color POSSIBLE_MOVE_COLOR = new Color(0, 255, 0, 128);

    public GuiView(@NotNull GuiController controller, @NotNull Board board) {
        this.controller = controller;
        this.board = board;

        initializeGUI();
        setupEventHandlers();
        updateBoard();
    }

    private void initializeGUI() {
        setTitle("Chess");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        boardPanel = new JPanel(new GridLayout(8, 8));
        squares = new JButton[8][8];

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                JButton square = new JButton();
                square.setPreferredSize(new Dimension(80, 80));
                square.setFont(new Font("Serif", Font.BOLD, 40));
                square.setFocusPainted(false);

                if ((row + col) % 2 == 0) {
                    square.setBackground(LIGHT_SQUARE);
                } else {
                    square.setBackground(DARK_SQUARE);
                }

                squares[row][col] = square;
                boardPanel.add(square);
            }
        }

        controlPanel = new JPanel(new BorderLayout());

        JPanel connectionPanel = new JPanel(new FlowLayout());
        connectionPanel.add(new JLabel("Host:"));
        hostField = new JTextField("localhost", 10);
        connectionPanel.add(hostField);
        connectionPanel.add(new JLabel("Port:"));
        portField = new JTextField("8080", 5);
        connectionPanel.add(portField);

        hostButton = new JButton("Host Game");
        joinButton = new JButton("Join Game");
        connectionPanel.add(hostButton);
        connectionPanel.add(joinButton);

        JPanel messagePanel = new JPanel(new BorderLayout());
        messageField = new JTextField();
        sendMessageButton = new JButton("Send");
        JPanel msgInputPanel = new JPanel(new BorderLayout());
        msgInputPanel.add(messageField, BorderLayout.CENTER);
        msgInputPanel.add(sendMessageButton, BorderLayout.EAST);

        messageArea = new JTextArea(10, 30);
        messageArea.setEditable(false);
        messageArea.setFont(new Font(Font.MONOSPACED, Font.PLAIN, 12));
        JScrollPane scrollPane = new JScrollPane(messageArea);

        messagePanel.add(msgInputPanel, BorderLayout.NORTH);
        messagePanel.add(scrollPane, BorderLayout.CENTER);

        statusLabel = new JLabel("Click 'Host Game' or 'Join Game' to start");
        statusLabel.setBorder(BorderFactory.createEmptyBorder(5, 10, 5, 10));

        controlPanel.add(connectionPanel, BorderLayout.NORTH);
        controlPanel.add(messagePanel, BorderLayout.CENTER);

        add(boardPanel, BorderLayout.CENTER);
        add(controlPanel, BorderLayout.EAST);
        add(statusLabel, BorderLayout.SOUTH);

        pack();
        setLocationRelativeTo(null);
    }

    private void setupEventHandlers() {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                final int finalRow = row;
                final int finalCol = col;
                squares[row][col].addActionListener(e -> handleSquareClick(finalCol, 7 - finalRow));
            }
        }

        hostButton.addActionListener(e -> {
            try {
                int port = Integer.parseInt(portField.getText().trim());
                controller.host(port);
            } catch (NumberFormatException ex) {
                showMessage("Invalid port number");
            }
        });

        joinButton.addActionListener(e -> {
            try {
                String host = hostField.getText().trim();
                int port = Integer.parseInt(portField.getText().trim());
                controller.join(host, port);
            } catch (NumberFormatException ex) {
                showMessage("Invalid port number");
            }
        });

        sendMessageButton.addActionListener(e -> sendMessage());
        messageField.addActionListener(e -> sendMessage());
    }

    private void handleSquareClick(int x, int y) {
        if (!controller.isGameStarted()) {
            return;
        }

        Coordinates clickedSquare = new Coordinates(x, y);

        if (selectedSquare == null) {
            Piece piece = board.getPieceAt(clickedSquare);
            if (piece != null && piece.getSide() == controller.getSide()) {
                selectedSquare = clickedSquare;
                highlightSquare(x, y, true);
                highlightPossibleMoves(piece);
            }
        } else {
            if (clickedSquare.equals(selectedSquare)) {
                clearHighlights();
                selectedSquare = null;
            } else {
                String move = coordinatesToChessNotation(selectedSquare) + " " + coordinatesToChessNotation(clickedSquare);
                controller.attemptMove(move);
                clearHighlights();
                selectedSquare = null;
            }
        }
    }

    private void highlightSquare(int x, int y, boolean selected) {
        JButton square = squares[7 - y][x];
        if (selected) {
            square.setBackground(SELECTED_COLOR);
        }
    }

    private void highlightPossibleMoves(Piece piece) {
        for (Coordinates move : piece.getMoves(board, piece.getCoordinates())) {
            JButton square = squares[7 - move.y()][move.x()];
            Color originalColor = square.getBackground();
            square.setBackground(POSSIBLE_MOVE_COLOR);
        }
    }

    private void clearHighlights() {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if ((row + col) % 2 == 0) {
                    squares[row][col].setBackground(LIGHT_SQUARE);
                } else {
                    squares[row][col].setBackground(DARK_SQUARE);
                }
            }
        }
    }

    private String coordinatesToChessNotation(Coordinates coords) {
        char file = (char) ('a' + coords.x());
        char rank = (char) ('1' + coords.y());
        return "" + file + rank;
    }

    private void sendMessage() {
        String message = messageField.getText().trim();
        if (!message.isEmpty()) {
            controller.sendMessage(message);
            messageField.setText("");
        }
    }

    public void updateBoard() {
        HashMap<Coordinates, Piece> pieces = board.getMap();

        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                squares[row][col].setText("");
            }
        }

        for (Coordinates coord : pieces.keySet()) {
            Piece piece = pieces.get(coord);
            JButton square = squares[7 - coord.y()][coord.x()];
            square.setText(getPieceSymbol(piece));

            if (piece.getSide() == Side.WHITE) {
                square.setForeground(Color.WHITE);
            } else {
                square.setForeground(Color.BLACK);
            }
        }

        updateStatus();
    }

    private String getPieceSymbol(Piece piece) {
        String symbol = "";
        switch (piece.getType()) {
            case King:
                symbol = piece.getSide() == Side.WHITE ? "♔" : "♚";
                break;
            case Queen:
                symbol = piece.getSide() == Side.WHITE ? "♕" : "♛";
                break;
            case Rook:
                symbol = piece.getSide() == Side.WHITE ? "♖" : "♜";
                break;
            case Bishop:
                symbol = piece.getSide() == Side.WHITE ? "♗" : "♝";
                break;
            case Knight:
                symbol = piece.getSide() == Side.WHITE ? "♘" : "♞";
                break;
            case Pawn:
                symbol = piece.getSide() == Side.WHITE ? "♙" : "♟";
                break;
        }
        return symbol;
    }

    private void updateStatus() {
        if (!controller.isGameStarted()) {
            statusLabel.setText("Waiting to start game...");
            return;
        }

        String status = "Current turn: " + board.getCurrentMove();

        if (board.isGameEnded()) {
            if (board.isPat()) {
                status = "Game ended in stalemate";
            } else if (board.isWhiteChecked()) {
                status = "Checkmate! Black wins";
            } else if (board.isBlackChecked()) {
                status = "Checkmate! White wins";
            }
        } else {
            if (board.isWhiteChecked()) {
                status += " (White in check)";
            } else if (board.isBlackChecked()) {
                status += " (Black in check)";
            }
        }

        statusLabel.setText(status);
    }

    public void showMessage(String message) {
        SwingUtilities.invokeLater(() -> {
            messageArea.append(message + "\n");
            messageArea.setCaretPosition(messageArea.getDocument().getLength());
        });
    }

    public void setConnectionEnabled(boolean enabled) {
        SwingUtilities.invokeLater(() -> {
            hostButton.setEnabled(enabled);
            joinButton.setEnabled(enabled);
            hostField.setEnabled(enabled);
            portField.setEnabled(enabled);
        });
    }

    public void setMessageEnabled(boolean enabled) {
        SwingUtilities.invokeLater(() -> {
            sendMessageButton.setEnabled(enabled);
            messageField.setEnabled(enabled);
        });
    }
}