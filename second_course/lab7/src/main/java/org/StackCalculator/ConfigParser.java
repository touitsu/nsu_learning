package org.StackCalculator;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public final class ConfigParser {
    private final String path;

    public ConfigParser(String path) {
        this.path = path;
    }

    public String getPath() {
        return this.path;
    }

    public String[] getCommands() throws RuntimeException {
        BufferedReader reader;
        String[] res;

        try {
            reader = new BufferedReader(new FileReader(this.path));
        }
        catch (IOException e) {
            throw new RuntimeException("Can not open file " + this.path + ".");
        }

        res = reader.lines().toList().toArray(new String[0]);

        try {
            reader.close();
        }
        catch (IOException e) {
            throw new RuntimeException("Error closing " + path + " file.");
        }

        return res;
    }
}
