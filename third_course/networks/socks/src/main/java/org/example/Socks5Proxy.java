package org.example;

import org.xbill.DNS.*;
import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

import org.xbill.DNS.ARecord;
import org.xbill.DNS.CNAMERecord;

public class Socks5Proxy {
    private final int port;
    private Selector selector;
    private ServerSocketChannel serverChannel;
    private DatagramChannel dnsChannel;
    private final ByteBuffer dnsBuffer = ByteBuffer.allocate(512);
    private final Map<Integer, ClientContext> pendingDnsQueries = new ConcurrentHashMap<>();
    private int nextDnsId = 1;

    public Socks5Proxy(int port) {
        this.port = port;
    }

    public void start() throws IOException {
        selector = Selector.open();

        
        serverChannel = ServerSocketChannel.open();
        serverChannel.configureBlocking(false);
        serverChannel.bind(new InetSocketAddress(port));
        serverChannel.register(selector, SelectionKey.OP_ACCEPT);

        
        dnsChannel = DatagramChannel.open();
        dnsChannel.configureBlocking(false);
        dnsChannel.connect(new InetSocketAddress("77.88.8.8", 53));
        dnsChannel.register(selector, SelectionKey.OP_READ);

        System.out.println("SOCKS5 proxy started on port " + port);
        System.out.println("Ready to accept connections...");

        while (true) {
            selector.select();
            Iterator<SelectionKey> keys = selector.selectedKeys().iterator();

            while (keys.hasNext()) {
                SelectionKey key = keys.next();
                keys.remove();

                if (!key.isValid()) continue;

                try {
                    if (key.isAcceptable()) {
                        acceptClient(key);
                    } else if (key.isConnectable()) {
                        finishConnection(key);
                    } else if (key.isReadable()) {
                        if (key.channel() == dnsChannel) {
                            handleDnsResponse();
                        } else {
                            handleRead(key);
                        }
                    } else if (key.isWritable()) {
                        handleWrite(key);
                    }
                } catch (Exception e) {
                    System.err.println("Error handling connection: " + e.getMessage());
                    closeConnection(key);
                }
            }
        }
    }

    private void acceptClient(SelectionKey key) throws IOException {
        ServerSocketChannel server = (ServerSocketChannel) key.channel();
        SocketChannel client = server.accept();
        client.configureBlocking(false);
        SelectionKey clientKey = client.register(selector, SelectionKey.OP_READ);
        ClientContext ctx = new ClientContext();
        ctx.clientChannel = client;
        ctx.clientKey = clientKey;
        clientKey.attach(ctx);

        System.out.println("New client connected from: " +
                ((SocketChannel)clientKey.channel()).socket().getRemoteSocketAddress());
    }

    private void finishConnection(SelectionKey key) throws IOException {
        SocketChannel channel = (SocketChannel) key.channel();
        ClientContext ctx = (ClientContext) key.attachment();

        if (channel.finishConnect()) {
            
            ByteBuffer response = ByteBuffer.allocate(10);
            response.put((byte) 0x05); 
            response.put((byte) 0x00); 
            response.put((byte) 0x00); 
            response.put((byte) 0x01); 
            response.put((byte) 0x00); 
            response.put((byte) 0x00);
            response.put((byte) 0x00);
            response.put((byte) 0x00);
            response.putShort((short) 0); 
            response.flip();

            ctx.clientBuffer = response;
            ctx.clientKey.interestOps(SelectionKey.OP_WRITE);
            ctx.targetKey.interestOps(SelectionKey.OP_READ);

            ctx.state = State.CONNECTED;

            System.out.println("Connected to target: " +
                    channel.socket().getRemoteSocketAddress());
        }
    }

    private void handleRead(SelectionKey key) throws IOException {
        ClientContext ctx = (ClientContext) key.attachment();

        if (ctx == null) {
            closeConnection(key);
            return;
        }

        if (key.channel() == ctx.clientChannel) {
            handleClientRead(key, ctx);
        } else if (key.channel() == ctx.targetChannel) {
            handleTargetRead(key, ctx);
        }
    }

    private void handleClientRead(SelectionKey key, ClientContext ctx) throws IOException {
        ByteBuffer buffer = ByteBuffer.allocate(8192);
        int read = ctx.clientChannel.read(buffer);

        if (read == -1) {
            System.out.println("Client closed connection");
            closeConnection(key);
            return;
        }

        if (read == 0) return;

        buffer.flip();

        switch (ctx.state) {
            case INIT:
                handleHandshake(buffer, ctx, key);
                break;
            case HANDSHAKE_DONE:
                handleRequest(buffer, ctx, key);
                break;
            case CONNECTED:
                forwardData(buffer, ctx, true);
                break;
            default:
                break;
        }
    }


    private void handleHandshake(ByteBuffer buffer, ClientContext ctx, SelectionKey key) throws IOException {
        if (buffer.remaining() < 2) return;

        byte version = buffer.get();
        int nmethods = buffer.get() & 0xFF;

        if (version != 0x05) {
            closeConnection(key);
            return;
        }

        if (buffer.remaining() < nmethods) {
            return;
        }

        boolean supportsNoAuth = false;
        for (int i = 0; i < nmethods; i++) {
            if (buffer.get() == 0x00) {
                supportsNoAuth = true;
                break;
            }
        }

        ByteBuffer response = ByteBuffer.allocate(2);
        response.put((byte) 0x05);

        if (supportsNoAuth) {
            response.put((byte) 0x00);
            response.flip();

            ctx.clientBuffer = response;
            key.interestOps(SelectionKey.OP_WRITE);
            ctx.state = State.HANDSHAKE_DONE;
        } else {
            response.put((byte) 0xFF);
            response.flip();

            SocketChannel channel = (SocketChannel) key.channel();
            channel.write(response);

            System.out.println("Client doesn't support NO AUTH method, closing connection");
            closeConnection(key);
        }
    }

    private void handleRequest(ByteBuffer buffer, ClientContext ctx, SelectionKey key) throws IOException {
        if (buffer.remaining() < 4) return;

        byte version = buffer.get();
        byte cmd = buffer.get();
        buffer.get(); 
        byte atyp = buffer.get();

        if (version != 0x05 || cmd != 0x01) { 
            sendError(key, (byte) 0x07); 
            return;
        }

        InetAddress address = null;
        int port = 0;

        switch (atyp) {
            case 0x01: 
                if (buffer.remaining() < 6) return;
                byte[] ipv4 = new byte[4];
                buffer.get(ipv4);
                address = InetAddress.getByAddress(ipv4);
                port = buffer.getShort() & 0xFFFF;
                break;

            case 0x03: 
                if (buffer.remaining() < 1) return;
                int len = buffer.get() & 0xFF;
                if (buffer.remaining() < len + 2) return;
                byte[] domainBytes = new byte[len];
                buffer.get(domainBytes);
                String domain = new String(domainBytes);
                port = buffer.getShort() & 0xFFFF;

                System.out.println("Resolving domain: " + domain + ":" + port);
                
                resolveDomain(domain, port, ctx, key);
                return;

            default:
                sendError(key, (byte) 0x08); 
                return;
        }

        connectToTarget(address, port, ctx, key);
    }

    private void resolveDomain(String domain, int port, ClientContext ctx, SelectionKey key) throws IOException {
        try {
            Message query = Message.newQuery(Record.newRecord(
                    Name.fromString(domain + "."), Type.A, DClass.IN));

            int dnsId = nextDnsId++;
            query.getHeader().setID(dnsId);

            byte[] queryData = query.toWire();
            ByteBuffer dnsQuery = ByteBuffer.wrap(queryData);

            pendingDnsQueries.put(dnsId, ctx);
            ctx.pendingDnsId = dnsId;
            ctx.targetPort = port;
            ctx.targetDomain = domain;
            ctx.state = State.RESOLVING;

            dnsChannel.write(dnsQuery);
        } catch (Exception e) {
            System.err.println("DNS query error: " + e.getMessage());
            sendError(key, (byte) 0x04); 
        }
    }

    private void handleDnsResponse() throws IOException {
        dnsBuffer.clear();
        int read = dnsChannel.read(dnsBuffer);
        if (read <= 0) return;

        dnsBuffer.flip();
        byte[] data = new byte[dnsBuffer.remaining()];
        dnsBuffer.get(data);

        try {
            Message response = new Message(data);
            int id = response.getHeader().getID();
            ClientContext ctx = pendingDnsQueries.remove(id);

            if (ctx != null) {
                
                org.xbill.DNS.Record[] answers = response.getSectionArray(Section.ANSWER);
                InetAddress resolvedAddress = findARecord(answers);

                if (resolvedAddress != null) {
                    System.out.println("Resolved " + ctx.targetDomain + " to " + resolvedAddress.getHostAddress());
                    connectToTarget(resolvedAddress, ctx.targetPort, ctx, ctx.clientKey);
                } else {
                    System.err.println("DNS resolution failed for: " + ctx.targetDomain);
                    sendError(ctx.clientKey, (byte) 0x04);
                }
            }
        } catch (Exception e) {
            System.err.println("Error parsing DNS response: " + e.getMessage());
        }
    }

    private InetAddress findARecord(org.xbill.DNS.Record[] records) {
        for (org.xbill.DNS.Record record : records) {
            if (record instanceof ARecord) {
                return ((ARecord) record).getAddress();
            } else if (record instanceof CNAMERecord) {
                
                
                System.out.println("Following CNAME: " + ((CNAMERecord) record).getTarget());
            }
        }
        return null;
    }

    private void connectToTarget(InetAddress address, int port, ClientContext ctx, SelectionKey key) throws IOException {
        try {
            SocketChannel targetChannel = SocketChannel.open();
            targetChannel.configureBlocking(false);

            if (targetChannel.connect(new InetSocketAddress(address, port))) {
                
                ctx.targetChannel = targetChannel;
                ctx.state = State.CONNECTED;
                SelectionKey targetKey = targetChannel.register(selector, SelectionKey.OP_READ);
                targetKey.attach(ctx);
                ctx.targetKey = targetKey;

                
                ByteBuffer response = ByteBuffer.allocate(10);
                response.put((byte) 0x05);
                response.put((byte) 0x00);
                response.put((byte) 0x00);
                response.put((byte) 0x01);
                response.put(address.getAddress());
                response.putShort((short) port);
                response.flip();

                ctx.clientBuffer = response;
                key.interestOps(SelectionKey.OP_WRITE);

                System.out.println("Direct connection to " + address.getHostAddress() + ":" + port);
            } else {
                ctx.targetChannel = targetChannel;
                ctx.state = State.CONNECTING;
                SelectionKey targetKey = targetChannel.register(selector, SelectionKey.OP_CONNECT);
                targetKey.attach(ctx);
                ctx.targetKey = targetKey;
            }
        } catch (IOException e) {
            System.err.println("Connection failed to " + address.getHostAddress() + ":" + port + ": " + e.getMessage());
            sendError(key, (byte) 0x04); 
        }
    }

    private void handleTargetRead(SelectionKey key, ClientContext ctx) throws IOException {
        ByteBuffer buffer = ByteBuffer.allocate(8192);
        int read = ctx.targetChannel.read(buffer);

        if (read == -1) {
            System.out.println("Target closed connection");
            closeConnection(key);
            return;
        }

        if (read == 0) return;

        buffer.flip();
        forwardData(buffer, ctx, false);
    }

    private void forwardData(ByteBuffer data, ClientContext ctx, boolean fromClient) {
        if (fromClient) {
            if (ctx.targetBuffer == null) {
                ctx.targetBuffer = data;
                if (ctx.targetKey != null) {
                    ctx.targetKey.interestOps(SelectionKey.OP_WRITE);
                }
            } else {
                
                ByteBuffer newBuffer = ByteBuffer.allocate(
                        ctx.targetBuffer.remaining() + data.remaining());
                newBuffer.put(ctx.targetBuffer);
                newBuffer.put(data);
                newBuffer.flip();
                ctx.targetBuffer = newBuffer;
            }

            
            if (ctx.targetBuffer != null && ctx.targetBuffer.remaining() > 65536 && ctx.clientKey != null) {
                ctx.clientKey.interestOps(SelectionKey.OP_WRITE);
            }
        } else {
            if (ctx.clientBuffer == null) {
                ctx.clientBuffer = data;
                if (ctx.clientKey != null) {
                    ctx.clientKey.interestOps(SelectionKey.OP_WRITE);
                }
            } else {
                ByteBuffer newBuffer = ByteBuffer.allocate(
                        ctx.clientBuffer.remaining() + data.remaining());
                newBuffer.put(ctx.clientBuffer);
                newBuffer.put(data);
                newBuffer.flip();
                ctx.clientBuffer = newBuffer;
            }

            if (ctx.clientBuffer != null && ctx.clientBuffer.remaining() > 65536 && ctx.targetKey != null) {
                ctx.targetKey.interestOps(SelectionKey.OP_WRITE);
            }
        }
    }

    private void handleWrite(SelectionKey key) throws IOException {
        ClientContext ctx = (ClientContext) key.attachment();

        if (ctx == null) {
            closeConnection(key);
            return;
        }

        if (key.channel() == ctx.clientChannel) {
            if (ctx.clientBuffer != null) {
                int written = ctx.clientChannel.write(ctx.clientBuffer);
                if (ctx.clientBuffer.remaining() == 0) {
                    ctx.clientBuffer = null;
                    if (ctx.state == State.CONNECTED || ctx.state == State.HANDSHAKE_DONE) {
                        key.interestOps(SelectionKey.OP_READ);
                    }
                    
                    if (ctx.targetKey != null &&
                            (ctx.targetBuffer == null || ctx.targetBuffer.remaining() < 32768)) {
                        ctx.targetKey.interestOps(SelectionKey.OP_READ);
                    }
                }
            } else {
                key.interestOps(SelectionKey.OP_READ);
            }
        } else if (key.channel() == ctx.targetChannel) {
            if (ctx.targetBuffer != null) {
                int written = ctx.targetChannel.write(ctx.targetBuffer);
                if (ctx.targetBuffer.remaining() == 0) {
                    ctx.targetBuffer = null;
                    key.interestOps(SelectionKey.OP_READ);

                    if (ctx.clientKey != null &&
                            (ctx.clientBuffer == null || ctx.clientBuffer.remaining() < 32768)) {
                        ctx.clientKey.interestOps(SelectionKey.OP_READ);
                    }
                }
            } else {
                key.interestOps(SelectionKey.OP_READ);
            }
        }
    }

    private void sendError(SelectionKey key, byte errorCode) throws IOException {
        ByteBuffer response = ByteBuffer.allocate(10);
        response.put((byte) 0x05);
        response.put(errorCode);
        response.put((byte) 0x00);
        response.put((byte) 0x01);
        response.put(new byte[4]);
        response.putShort((short) 0);
        response.flip();

        SocketChannel channel = (SocketChannel) key.channel();
        channel.write(response);
        closeConnection(key);
    }

    private void closeConnection(SelectionKey key) {
        try {
            ClientContext ctx = (ClientContext) key.attachment();

            if (ctx != null) {
                if (ctx.pendingDnsId != 0) {
                    pendingDnsQueries.remove(ctx.pendingDnsId);
                }

                if (ctx.targetKey != null) {
                    ctx.targetKey.cancel();
                    try { ctx.targetChannel.close(); } catch (IOException ignored) {}
                }

                if (ctx.clientKey != null && ctx.clientKey != key) {
                    ctx.clientKey.cancel();
                    try { ctx.clientChannel.close(); } catch (IOException ignored) {}
                }
            }

            key.cancel();
            key.channel().close();
        } catch (IOException ignored) {}
    }

    private static class ClientContext {
        State state = State.INIT;
        SocketChannel clientChannel;
        SocketChannel targetChannel;
        SelectionKey clientKey;
        SelectionKey targetKey;
        ByteBuffer clientBuffer;
        ByteBuffer targetBuffer;
        int pendingDnsId;
        int targetPort;
        String targetDomain;
    }

    private enum State {
        INIT,
        HANDSHAKE_DONE,
        RESOLVING,
        CONNECTING,
        CONNECTED
    }
}