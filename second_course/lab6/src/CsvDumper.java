import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Map;

public final class CsvDumper {
    private final String path;

    public CsvDumper(String path) {
        this.path = path;
    }

    public void dump(Map<String, Integer> map, int totalWordsCnt) {

        try (FileOutputStream file = new FileOutputStream(this.path, false)){
            file.write("Word;Frequency;Percentage\n".getBytes());

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
