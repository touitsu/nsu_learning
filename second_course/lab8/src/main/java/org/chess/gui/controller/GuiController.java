package org.chess.gui.controller;

import org.chess.connection.Message;
import org.chess.console.exceptions.InputFormatException;
import org.chess.console.exceptions.NoInputException;
import org.chess.console.exceptions.UnknownCommandException;
import org.chess.controller.Controller;
import org.chess.gui.view.GuiView;
import org.chess.model.Board;
import org.chess.model.Coordinates;
import org.chess.model.exceptions.MoveUnavailableException;
import org.chess.model.pieces.Piece;
import org.chess.model.pieces.Side;
import org.chess.player.Player;
import org.jetbrains.annotations.NotNull;

import javax.swing.*;
import java.io.IOException;
import java.util.HashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class GuiController extends Controller {

    private final Board board;
    private GuiView view;
    private boolean isHost = false;
    private static final Pattern movePattern = Pattern.compile("[a-h][1-8] ?[a-h][1-8].*");

    public GuiController(@NotNull Board board, @NotNull Player player) {
        super(player);
        this.board = board;
    }

    public void setView(GuiView view) {
        this.view = view;
    }

    @Override
    public void handleInput() throws MoveUnavailableException, UnknownCommandException, NoInputException {
        // GUI handles input through event listeners, so this method is not used
    }

    @Override
    public void handleDisconnection() {
        SwingUtilities.invokeLater(() -> {
            view.showMessage("Opponent disconnected, waiting for reconnect...");
            view.setConnectionEnabled(true);
            view.setMessageEnabled(false);
        });
    }

    @Override
    public void host(int port) {
        this.side = Side.WHITE;

        SwingUtilities.invokeLater(() -> {
            view.showMessage("Waiting for opponent...");
            view.setConnectionEnabled(false);
        });

        new Thread(() -> {
            try {
                this.networkManager.host(port);
                this.isHost = true;
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Successfully hosted game on port " + port);
                    view.setMessageEnabled(true);
                });
                sendStartGame();
            } catch (IOException e) {
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Failed to host game: " + e.getMessage());
                    view.setConnectionEnabled(true);
                });
            }
        }).start();
    }

    @Override
    public void join(@NotNull String host, int port) {
        this.side = Side.BLACK;

        SwingUtilities.invokeLater(() -> {
            view.showMessage("Connecting to " + host + ":" + port + "...");
            view.setConnectionEnabled(false);
        });

        new Thread(() -> {
            try {
                this.networkManager.join(host, port);
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Successfully joined game");
                    view.setMessageEnabled(true);
                });
                sendStartGame();
            } catch (IOException e) {
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Failed to join game: " + e.getMessage());
                    view.setConnectionEnabled(true);
                });
            }
        }).start();
    }

    @Override
    public void handleOpponentMove(@NotNull Message message) {
        try {
            String move = validateMove((String) message.getData());
            SwingUtilities.invokeLater(() -> {
                view.showMessage(message.getSender().getName() + " made move: " + message.getData());
            });

            board.move(move, this.side.opposite());

            SwingUtilities.invokeLater(() -> {
                view.updateBoard();
            });

            try {
                if (this.board.isGameEnded()) {
                    if (this.board.isPat()) {
                        sendEndGame("Pat");
                    } else if (this.board.isBlackChecked()) {
                        sendEndGame("Black mated");
                    } else {
                        sendEndGame("White mated");
                    }
                }
            } catch (IOException e) {
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Failed to send end game: " + e.getMessage());
                });
            }

        } catch (MoveUnavailableException | InputFormatException e) {
            SwingUtilities.invokeLater(() -> {
                view.showMessage("Opponent sent an invalid move: " + e.getMessage());
            });
        }
    }

    @Override
    public void sendMove(@NotNull String move) {
        try {
            this.networkManager.sendMove(move);

            if (this.board.isGameEnded()) {
                if (this.board.isPat()) {
                    sendEndGame("Pat");
                } else if (this.board.isBlackChecked()) {
                    sendEndGame("Black mated");
                } else {
                    sendEndGame("White mated");
                }
            }
        } catch (IOException e) {
            SwingUtilities.invokeLater(() -> {
                view.showMessage("Failed to send move: " + e.getMessage());
            });
        }
    }

    @Override
    public void sendMessage(@NotNull String message) {
        try {
            this.networkManager.sendMessage(message);
            SwingUtilities.invokeLater(() -> {
                view.showMessage("You: " + message);
            });
        } catch (IOException e) {
            SwingUtilities.invokeLater(() -> {
                view.showMessage("Failed to send message: " + e.getMessage());
            });
        } catch (NullPointerException e) {
            SwingUtilities.invokeLater(() -> {
                view.showMessage("Failed to send message: Not connected");
            });
        }
    }

    @Override
    public void handleMessage(@NotNull Message message) {
        SwingUtilities.invokeLater(() -> {
            view.showMessage(message.getSender().getName() + ": " + message.getData());
        });
    }

    @Override
    public void sendStartGame() throws IOException {
        this.networkManager.sendStartGame();
    }

    @Override
    public void handleStartGame(@NotNull Message message) {
        if (this.gameStarted) {
            try {
                this.sendSync();
            } catch (IOException e) {
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Failed to synchronize: " + e.getMessage());
                });
            }
        } else {
            this.gameStarted = true;
            SwingUtilities.invokeLater(() -> {
                if (this.isHost) {
                    view.showMessage("Player " + message.getSender().getName() + " joined your game");
                } else {
                    view.showMessage("You joined " + message.getSender().getName() + "'s game");
                }
                view.updateBoard();
            });
        }
    }

    @Override
    public void sendContinueGame() throws IOException {
        this.networkManager.sendContinueGame();
    }

    @Override
    public void handleContinueGame() {
        this.board.setGameEnded(false);
        SwingUtilities.invokeLater(() -> {
            view.updateBoard();
        });
    }

    @Override
    public void sendEndGame(@NotNull String reason) throws IOException {
        this.networkManager.sendEndGame(reason);
    }

    @Override
    public void handleEndGame(@NotNull Message message) {
        String reason = (String) message.getData();

        if (reason.equals("Pat")) {
            if (this.board.isPat()) {
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Game ended in stalemate");
                });
                endGame();
            } else {
                try {
                    sendContinueGame();
                } catch (IOException e) {
                    SwingUtilities.invokeLater(() -> {
                        view.showMessage("Failed to continue game: " + e.getMessage());
                    });
                }
            }
        } else if (reason.equals("Black mated")) {
            if (this.board.isBlackChecked() && this.board.isGameEnded()) {
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("Black is checkmated");
                });
                endGame();
            } else {
                try {
                    sendContinueGame();
                } catch (IOException e) {
                    SwingUtilities.invokeLater(() -> {
                        view.showMessage("Failed to continue game: " + e.getMessage());
                    });
                }
            }
        } else {
            if (this.board.isWhiteChecked() && this.board.isGameEnded()) {
                SwingUtilities.invokeLater(() -> {
                    view.showMessage("White is checkmated");
                });
                endGame();
            } else {
                try {
                    sendContinueGame();
                } catch (IOException e) {
                    SwingUtilities.invokeLater(() -> {
                        view.showMessage("Failed to continue game: " + e.getMessage());
                    });
                }
            }
        }
    }

    @Override
    public void endGame() {
        try {
            this.gameRunning = false;
            SwingUtilities.invokeLater(() -> {
                view.showMessage("Game ended. You can start a new game.");
                view.setConnectionEnabled(true);
                view.setMessageEnabled(false);
            });
            this.networkManager.close();
        } catch (IOException e) {
            SwingUtilities.invokeLater(() -> {
                view.showMessage("Failed to close network connection: " + e.getMessage());
            });
        }
    }

    @Override
    public void sync(@NotNull Message message) {
        @SuppressWarnings("unchecked")
        HashMap<Coordinates, Piece> map = (HashMap<Coordinates, Piece>) message.getData();
        this.board.setMap(map);
        SwingUtilities.invokeLater(() -> {
            view.updateBoard();
        });
    }

    @Override
    public void sendSync() throws IOException {
        this.networkManager.sendSync(this.board.getMap());
    }

    public void attemptMove(String move) {
        try {
            String validatedMove = validateMove(move);
            board.move(validatedMove, this.side);
            sendMove(validatedMove);
            SwingUtilities.invokeLater(() -> {
                view.updateBoard();
            });
        } catch (MoveUnavailableException | InputFormatException e) {
            SwingUtilities.invokeLater(() -> {
                view.showMessage("Invalid move: " + e.getMessage());
            });
        }
    }

    public Side getSide() {
        return this.side;
    }

    public boolean isGameStarted() {
        return this.gameStarted;
    }

    public boolean isGameRunning() {
        return this.gameRunning;
    }

    private static String validateMove(@NotNull String str) throws InputFormatException {
        Matcher matcher;

        if (str.isEmpty()) {
            throw new InputFormatException("Input string is empty. (Correct input is \"[a-h][1-8] ?[a-h][1-8]\")");
        }

        matcher = movePattern.matcher(str);

        if (matcher.matches()) {
            return str.indexOf(' ') == -1 ? str.substring(0, 2) + ' ' + str.substring(2, 4) : str.substring(0, 5);
        } else {
            throw new InputFormatException("Input string doesn't match regex. (Correct input is \"[a-h][1-8] ?[a-h][1-8]\")");
        }
    }
}