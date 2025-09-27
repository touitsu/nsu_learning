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
import java.net.SocketTimeoutException;
import java.util.HashMap;
import java.util.concurrent.atomic.AtomicBoolean;

public class NetworkManager {
    private Socket socket;
    private ServerSocket serverSocket;
    private ObjectOutputStream out;
    private ObjectInputStream in;
    private final Controller controller;
    private volatile boolean connected;
    private Thread listener;
    private volatile boolean isHost = false;
    private volatile boolean shutdown = false;
    private Thread reconnectThread;
    private String reconnectHost;
    private int reconnectPort;
    private volatile long lastReceivedTime;
    private Thread heartbeatThread;
    private final AtomicBoolean running = new AtomicBoolean(false);

    public NetworkManager(@NotNull Controller controller) {
        this.controller = controller;
    }


    private void listen() {
        running.set(true);
        lastReceivedTime = System.currentTimeMillis();

        try {
            while (running.get()) {
                try {
                    socket.setSoTimeout(1000);
                    Message message = (Message) in.readObject();
                    lastReceivedTime = System.currentTimeMillis();
                    processMessage(message);
                } catch (SocketTimeoutException e) {
                    if (System.currentTimeMillis() - lastReceivedTime > 10000) {
                        throw new IOException("Connection timeout");
                    }
                }
            }
        } catch (IOException | ClassNotFoundException e) {
            handleDisconnect();
        }
    }

    private void handleDisconnect() {
        running.set(false);
        this.controller.handleDisconnection();
        this.connected = false;

        if (!isHost && !shutdown) {
            startReconnectionThread();
        }
    }

    private void setStreams() throws IOException {
        this.out = new ObjectOutputStream(this.socket.getOutputStream());
        this.in = new ObjectInputStream(this.socket.getInputStream());

        this.listener = new Thread(this::listen);

        this.listener.start();

        startHeartbeat();

        this.connected = true;
    }


    public void host(int port) throws IOException {
        this.serverSocket = new ServerSocket(port);

        this.socket = serverSocket.accept();

        serverSocket.close();
        setStreams();
    }


    public void join(@NotNull String host, int port) throws IOException{
        this.isHost = false;
        this.shutdown = false;
        this.reconnectHost = host;
        this.reconnectPort = port;
        this.socket = new Socket(host, port);

        setStreams();
    }


    private void startHeartbeat() {
        heartbeatThread = new Thread(() -> {
            while (running.get() && !shutdown) {
                try {
                    Thread.sleep(5000); // Send heartbeat every 5 seconds
                    if (running.get() && connected) {
                        out.writeObject(new Message(MessageType.HEARTBEAT, null, null));
                        out.flush();
                    }
                } catch (InterruptedException | IOException e) {
                    if (!shutdown) {
                        handleDisconnect();
                    }
                    break;
                }
            }
        });
        heartbeatThread.setDaemon(true);
        heartbeatThread.start();
    }


    private void startReconnectionThread() {
        if (this.reconnectThread != null && this.reconnectThread.isAlive()) {
            return;
        }

        System.out.println("reconnecting");

        reconnectThread = new Thread(() -> {
            while (!this.shutdown) {
                for (int attempts = 0; attempts < 10; attempts++) {
                    try {
                        Thread.sleep(1000);
                        reconnect();
                        break;
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        break;
                    } catch (IOException ignore) {
                        System.out.println(ignore.getMessage());
                    }
                }
                try {
                    close();
                    this.controller.writeMessage("Failed to reconnect");
                    this.controller.endGame();
                } catch (IOException ignore) {
                    System.out.println("Failed to close");
                }
            }
        });
        reconnectThread.start();
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
            case REQUEST_SYNC:
                try {
                    this.controller.sendSync();
                }
                catch (IOException e) {
                    System.out.println("Failed to request sync.");
                }
                break;
            case SYNC:
                this.controller.handleSync(message);
                break;
            case HEARTBEAT:
                this.lastReceivedTime = System.currentTimeMillis();
                break;
        }
    }

    private void reconnect() throws IOException {
        this.socket = new Socket(reconnectHost, reconnectPort);
        setStreams();
        this.controller.handleReconnection();
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

    public void sendSyncRequest() throws IOException {
        this.out.writeObject(new Message(MessageType.REQUEST_SYNC, this.controller.getPlayer(), null));
    }

    public void close() throws IOException {
        this.shutdown = true;
        this.running.set(false);
        this.connected = false;

        if (listener != null) {
            listener.interrupt();
        }
        if (reconnectThread != null) {
            reconnectThread.interrupt();
        }
        if (heartbeatThread != null) {
            heartbeatThread.interrupt();
        }

        if (socket != null && !socket.isClosed()) {
            socket.close();
        }
        if (serverSocket != null && !serverSocket.isClosed()) {
            serverSocket.close();
        }
    }

    public boolean getConnected() {
        return this.connected;
    }
}
