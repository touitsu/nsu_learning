
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashSet;

public final class Parser {
    private final String path;

    public Parser(String path) {
        this.path = path;
    }

    public ArrayList<String> getLine(HashSet<Character> sep, int idx) {
        String str;
        StringBuilder tmp;
        ArrayList<String> tokens;

        try (BufferedReader file = new BufferedReader(new FileReader(this.path))) {
            for (int i = 0; i < idx; i++) {
                file.readLine();
            }

            str = file.readLine();

            if(str == null) {
                return null;
            }

            tokens = new ArrayList<>();

            tmp = new StringBuilder(new String());

            for (int i = 0; i < str.length(); i++) {
                if (sep.contains(str.charAt(i)) && !tmp.isEmpty()) {
                    tokens.add(tmp.toString());
                    tmp = new StringBuilder(new String());
                }
                else if (!sep.contains(str.charAt(i))) {
                    tmp.append(str.charAt(i));
                }
            }

            return tokens;

        }
        catch (FileNotFoundException ex) {
            throw new RuntimeException("Input file not found.");
        }
        catch (IOException ex) {
            throw new RuntimeException("Error closing input file.");
        }


    }
}