import java.io.FileOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.TreeSet;

public final class CsvDumper {
    private final String path;

    public CsvDumper(String path) {
        this.path = path;
    }

    public String getPath() {
        return this.path;
    }

    public void dump(TreeSet<Word> set) {
        FileOutputStream file;
        byte[] legend = "Word;Frequency;Percentage\n".getBytes();

        try {
            file = new FileOutputStream(this.path, false);
            file.write(legend);
        }
        catch (FileNotFoundException e) {
            throw new RuntimeException("Output file cannot be created.");
        }
        catch (IOException e) {
            throw new RuntimeException("Unable to write legend to the output file.");
        }

        for (Word w : set.descendingSet()) {
            try {
                for (int j = 0; j < w.toString().length(); j++) {
                    file.write(w.toString().charAt(j));
                }

                file.write(';');

                file.write(w.getFreq().toString().getBytes());

                file.write(';');

                file.write(String.format("%.2f", w.getPercentage()).getBytes());

                file.write('\n');
            }
            catch (IOException e) {
                throw new RuntimeException("Unable to write to file word " + w.toString() + ".");
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
