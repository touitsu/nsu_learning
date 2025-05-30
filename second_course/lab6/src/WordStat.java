
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

public final class WordStat {

    private final Map<String, Integer> workResult;
    private final int totalTokens;

    public WordStat(String inputPath, String seps) {
        Map<String, Integer> map;
        ArrayList<String> tokens;
        Parser parser;
        int totalTokens;
        HashSet<Character> separators;

        separators = new HashSet<>();
        for (char c : seps.toCharArray()) {
            separators.add(c);
        }

        map = new HashMap<>();
        parser = new Parser(inputPath);
        totalTokens = 0;


        tokens = parser.getLine(separators, 0);
        for (int i = 1; tokens != null; i++) {

            for (String token : tokens) {

                map.merge(token, 1, Integer::sum);

                totalTokens++;
            }

            tokens = parser.getLine(separators, i);
        }

        this.workResult = map;
        this.totalTokens = totalTokens;
    }

    public Map<String, Integer> getMap() {
        return this.workResult;
    }

    public int getTotalTokens() {
        return this.totalTokens;
    }
}
