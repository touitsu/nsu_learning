import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public final class Parser {
    private final String path;

    public Parser(String path) {
        this.path = path;
    }

    public String getPath() {
        return this.path;
    }

    public static boolean inArray(char c, char[] arr) {
        for (char value : arr) {
            if (value == c) {
                return true;
            }
        }

        return false;
    }

    public ArrayList<String> tokenize(char[] sep) {
        BufferedReader reader;
        ArrayList<String> tokens;
        List<String> allLines;

        try {
            reader = new BufferedReader(new FileReader(this.path));
        } catch (FileNotFoundException e) {
            throw new RuntimeException("File " + this.path + " is not found.");
        }

        tokens = new ArrayList<String>();
        allLines = reader.lines().toList();

        for (int i = 0, lidx = 0; i < allLines.size(); i++, lidx = 0) {
            for (int j = 0; j < allLines.get(i).length(); j++) {
                if (inArray(allLines.get(i).charAt(j), sep)) {
                    if (lidx != j) {
                        tokens.add(allLines.get(i).substring(lidx, j));
                    }

                    lidx = j + 1;
                }
            }

            if (lidx < allLines.get(i).length()) {
                tokens.add(allLines.get(i).substring(lidx));
            }
        }

        try {
            reader.close();
        }
        catch (IOException e) {
            throw new RuntimeException("Cannot close reader.");
        }

        return tokens;
    }
}