package org.StackCalculator;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public final class ConfigParser {
    private final String path;

    public ConfigParser(String path) {
        this.path = path;
    }

    public String getPath() {
        return this.path;
    }

    public String[] getCommands() throws IOException {
        BufferedReader reader;
        String[] res;

        try {
            reader = new BufferedReader(new FileReader(this.path));
        }
        catch (IOException e) {
            throw new IOException("Can not open file " + this.path + ".");
        }

        res = reader.lines().toList().toArray(new String[0]);

        try {
            reader.close();
        }
        catch (IOException e) {
            throw new IOException("Error closing " + path + " file.");
        }

        return res;
    }
}
