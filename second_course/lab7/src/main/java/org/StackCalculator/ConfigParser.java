package org.StackCalculator;

import org.Exceptions.ConfigException;
import org.Exceptions.ConfigFormatException;
import org.Exceptions.ConfigNotFoundException;

import java.io.*;

public final class ConfigParser {
    private final String path;
    private int linesRead;
    private BufferedReader reader;

    public ConfigParser(String path) {
        this.path = path;
        this.linesRead = 0;

    }

    public String getLine() throws ConfigException {
        String line;

        line = null;

        try (BufferedReader file = this.path.equals("Stream") ? new BufferedReader(new InputStreamReader(new InputStreamWrapper(System.in))) : new BufferedReader(new FileReader(this.path))) {

            for (int i = 0; i < this.linesRead && !this.path.equals("Stream"); i++) {
                file.readLine();
            }

            line = file.readLine();

            System.out.println(line.charAt(0));

            if (line.equals("exit") && this.path.equals("Stream")) {
                System.in.close();
                return null;
            }

            if (line == null) {
                return null;
            }

        }
        catch (FileNotFoundException e) {
            throw new ConfigNotFoundException(e.getMessage());
        }
        catch (IOException e) {
            if (!e.getMessage().equals("Stream closed")) {
                throw new ConfigFormatException(e.getMessage());
            }
        }

        this.linesRead++;

        return line;
    }
}
