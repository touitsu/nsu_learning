
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.stream.Collectors;

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
                if (map.containsKey(token)) {
                    map.replace(token, map.get(token), map.get(token) + 1);
                }
                else {
                    map.put(token, 1);
                }
                totalTokens++;
            }

            tokens = parser.getLine(separators, i);
        }

        map = map.entrySet()
                .stream()
                .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue, (oldValue, newValue) -> oldValue, LinkedHashMap::new));

        dumper.dump(map, totalTokens);
    }
}
