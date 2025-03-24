package org.StackCalculator;

import java.io.IOException;
import java.io.InputStream;

public final class InputStreamWrapper extends InputStream {
    private final InputStream stream;

    public InputStreamWrapper(InputStream stream) {
        this.stream = stream;
    }

    @Override
    public int read() throws IOException {
        return stream.read();
    }

    @Override
    public int read(byte[] b) throws IOException {
        return  stream.read(b);
    }

    @Override
    public int read(byte[] b, int off, int len) throws IOException {
        return  stream.read(b, off, len);
    }

    @Override
    public void close() throws IOException {

    }

}
