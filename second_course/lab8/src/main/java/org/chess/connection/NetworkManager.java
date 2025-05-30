package org.chess.connection;

import org.chess.controller.Controller;
import org.chess.model.Coordinates;
import org.chess.model.pieces.Piece;
import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;

public class NetworkManager {
    private Socket socket;
    private ServerSocket serverSocket;
    private ObjectOutputStream out;
    private ObjectInputStream in;
    private final Controller controller;
    private volatile boolean connected;
    private Thread listener;

    public NetworkManager(@NotNull Controller controller) {
        this.controller = controller;
    }


    private void listen() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                processMessage((Message) in.readObject());
            }
        }
        catch (IOException | ClassNotFoundException e) {
            this.controller.handleDisconnection();
            this.connected = false;
            this.listener.interrupt();
        }
    }


    private void setStreams() throws IOException {
        this.out = new ObjectOutputStream(this.socket.getOutputStream());
        this.in = new ObjectInputStream(this.socket.getInputStream());

        this.listener = new Thread(this::listen);

        this.listener.start();
        this.connected = true;
    }


    public void host(int port) throws IOException {
        this.serverSocket = new ServerSocket(port);

        this.socket = serverSocket.accept();

        setStreams();
    }


    public void join(@NotNull String host, int port) throws IOException{
        this.socket = new Socket(host, port);

        setStreams();
    }

    private void processMessage(@NotNull Message message) {
        switch (message.getType()) {
            case MOVE:
                this.controller.handleOpponentMove(message);
                break;
            case PLAYER_LEFT:
                this.controller.handleDisconnection();
                break;
            case MESSAGE:
                this.controller.handleMessage(message);
                break;
            case GAME_START:
                this.controller.handleStartGame(message);
                break;
            case GAME_END:
                this.controller.handleEndGame(message);
                break;
            case GAME_CONTINUE:
                this.controller.handleContinueGame();
                break;
            case SYNC:
                this.controller.sync(message);
                break;
        }
    }

    public void sendMove(@NotNull String move) throws IOException {
        this.out.writeObject(new Message(MessageType.MOVE, this.controller.getPlayer(), move));
    }

    public void sendMessage(String message) throws IOException {
        this.out.writeObject(new Message(MessageType.MESSAGE, this.controller.getPlayer(), message));
    }

    public void sendStartGame() throws IOException {
        this.out.writeObject(new Message(MessageType.GAME_START, this.controller.getPlayer(), null));
    }

    public void sendEndGame(String reason) throws IOException {
        this.out.writeObject(new Message(MessageType.GAME_END, this.controller.getPlayer(), reason));
    }

    public void sendContinueGame() throws IOException {
        this.out.writeObject(new Message(MessageType.GAME_CONTINUE, this.controller.getPlayer(), null));
    }

    public void sendSync(HashMap<Coordinates, Piece> map) throws IOException {
        this.out.writeObject(new Message(MessageType.SYNC, this.controller.getPlayer(), map));
    }

    public void close() throws IOException {
        if (this.socket != null) {
            this.socket.close();
            this.listener.interrupt();
        }
    }

    public boolean getConnected() {
        return this.connected;
    }
}
