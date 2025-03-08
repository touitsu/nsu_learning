import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Map;

public final class CsvDumper {
    private final String path;

    public CsvDumper(String path) {
        this.path = path;
    }

    public String getPath() {
        return this.path;
    }

    public void dump(Map<String, Integer> map, int totalWordsCnt) {
        FileOutputStream file;

        try {
            file = new FileOutputStream(this.path, false);
            file.write("Word;Frequency;Percentage\n".getBytes());
        }
        catch (FileNotFoundException e) {
            throw new RuntimeException("Output file cannot be created.");
        }
        catch (IOException e) {
            throw new RuntimeException("Unable to write legend to the output file.");
        }

        for (Map.Entry<String, Integer> entry: map.entrySet()) {
            try {
                file.write((entry.getKey() + ';' + entry.getValue() + ';' + String.format("%.2f", entry.getValue().doubleValue()/totalWordsCnt) + '\n').getBytes());
            }
            catch (IOException e) {
                throw new RuntimeException("Unable to write to file word " + entry.getKey() + ".");
            }
        }

        try {
            file.close();
        }
        catch (IOException e) {
            throw new RuntimeException("Unable to close output file.");
        }
    }
}
