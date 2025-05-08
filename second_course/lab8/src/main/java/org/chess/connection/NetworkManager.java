package org.chess.connection;

import org.chess.controller.Controller;

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
                Message message = (Message) in.readObject();
            }
        }
        catch (IOException | ClassNotFoundException e) {
            this.controller.handleDisconnection();
        }
    }


    private void setStreams() throws IOException {
        this.out = new ObjectOutputStream(this.socket.getOutputStream());
        this.in = new ObjectInputStream(this.socket.getInputStream());
    }


    public void host(int port) throws IOException {
        ServerSocket serverSocket = new ServerSocket(port);

        this.socket = serverSocket.accept();
    }
}
