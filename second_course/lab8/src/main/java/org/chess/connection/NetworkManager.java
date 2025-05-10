package org.chess.connection;

import org.chess.controller.Controller;
import org.chess.model.exceptions.MoveUnavailableException;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

public class NetworkManager {
    private Socket socket;
    private ObjectOutputStream out;
    private ObjectInputStream in;
    private final Controller controller;


    public NetworkManager(Controller controller) {
        this.controller = controller;
    }


    private void listen() {
        try {
            while (true) {
                processMessage((Message) in.readObject());
            }
        }
        catch (IOException | ClassNotFoundException e) {
            this.controller.handleDisconnection();
        }
    }


    private void setStreams() throws IOException {
        this.out = new ObjectOutputStream(this.socket.getOutputStream());
        this.in = new ObjectInputStream(this.socket.getInputStream());

        new Thread(this::listen).start();
    }


    public void host(int port) throws IOException {
        ServerSocket serverSocket = new ServerSocket(port);

        this.socket = serverSocket.accept();

        setStreams();
    }

    public void join(String host, int port) throws IOException{
        this.socket = new Socket(host, port);

        setStreams();
    }

    private void processMessage(Message message) {
        switch (message.getType()) {
            case MOVE:
                try {
                    this.controller.handleOpponentMove((String) message.getData());
                }
                catch (MoveUnavailableException e) {
                    System.out.println("Opponent sent unavailable move");
                }
                break;
            case PLAYER_LEFT:
                break;
            case MESSAGE:
                this.controller.receiveMessage(message);
                break;
            case GAME_START:
                this.controller.receiveStartGame(message);
        }
    }

    public void sendMove(String move) throws IOException {
        this.out.writeObject(new Message(MessageType.MOVE, this.controller.getPlayer(), move));
    }

    public void sendMessage(String message) throws IOException {
        this.out.writeObject(new Message(MessageType.MESSAGE, this.controller.getPlayer(), message));
    }

    public void sendStartGame() throws IOException {
        this.out.writeObject(new Message(MessageType.GAME_START, this.controller.getPlayer(), null));
    }

    public void close() throws IOException {
        if (this.socket != null) {
            this.socket.close();
        }
    }
}
