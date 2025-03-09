
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

public final class WordStat {

    public void complete(String inputPath, String outputPath, String seps) {
        Map<String, Integer> map;
        ArrayList<String> tokens;
        Parser parser;
        CsvDumper dumper;
        int totalTokens;
        HashSet<Character> separators;

        separators = new HashSet<>();
        for (char c : seps.toCharArray()) {
            separators.add(c);
        }

        map = new HashMap<>();
        parser = new Parser(inputPath);
        totalTokens = 0;
        dumper = new CsvDumper(outputPath);

        tokens = parser.getLine(separators, 0);
        for (int i = 1; tokens != null; i++) {

            for (String token : tokens) {
                if (map.replace(token, map.get(token) == null ? 1 : map.get(token) + 1) == null) {
                    map.put(token, 1);
                }

                totalTokens++;
            }

            tokens = parser.getLine(separators, i);
        }

        dumper.dump(map, totalTokens);
    }
}
