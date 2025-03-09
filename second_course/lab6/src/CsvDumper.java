import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.stream.Collectors;

public final class CsvDumper {
    private final String path;

    public CsvDumper(String path) {
        this.path = path;
    }

    public void dump(Map<String, Integer> map, int totalWordsCnt) {

        try (FileOutputStream file = new FileOutputStream(this.path, false)){
            file.write("Word;Frequency;Percentage\n".getBytes());

            map = map.entrySet()
                    .stream()
                    .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                    .collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue, (oldValue, newValue) -> oldValue, LinkedHashMap::new));

            for (Map.Entry<String, Integer> entry: map.entrySet()) {
                try {
                    file.write((entry.getKey() + ';' + entry.getValue() + ';' + String.format("%.2f", entry.getValue().doubleValue()/totalWordsCnt) + '\n').getBytes());
                }
                catch (IOException e) {
                    throw new RuntimeException("Unable to write to file word " + entry.getKey() + ".");
                }
            }
        }
        catch (FileNotFoundException e) {
            throw new RuntimeException("Output file cannot be created.");
        }
        catch (IOException e) {
            throw new RuntimeException("Unable to write to the output file.");
        }
    }
}
