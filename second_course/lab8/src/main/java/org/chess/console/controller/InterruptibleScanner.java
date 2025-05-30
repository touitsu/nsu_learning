package org.chess.console.controller;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.channels.Channels;
import java.nio.channels.ReadableByteChannel;
import java.nio.charset.StandardCharsets;

public class InterruptibleScanner implements Closeable {
    private final ReadableByteChannel channel;
    private final BufferedReader reader;

    public InterruptibleScanner(InputStream in) {
        this.channel = Channels.newChannel(in);
        this.reader = new BufferedReader(
                new InputStreamReader(
                        new ChannelInputStream(channel),
                        StandardCharsets.UTF_8
                )
        );
    }

    public String readLine() throws IOException {
        try {
            return reader.readLine();
        } catch (IOException e) {
            if (!channel.isOpen()) {
                throw new IOException("Scanner was closed");
            }
            throw e;
        }
    }

    @Override
    public void close() throws IOException {
        channel.close();
        reader.close();
    }

    private static class ChannelInputStream extends InputStream {
        private final ReadableByteChannel channel;

        public ChannelInputStream(ReadableByteChannel channel) {
            this.channel = channel;
        }

        @Override
        public int read() throws IOException {
            ByteBuffer buf = ByteBuffer.allocate(1);
            int read = channel.read(buf);
            if (read == -1) return -1;
            buf.flip();
            return buf.get() & 0xFF;
        }

        @Override
        public int read(byte[] b, int off, int len) throws IOException {
            ByteBuffer buf = ByteBuffer.wrap(b, off, len);
            return channel.read(buf);
        }
    }
}