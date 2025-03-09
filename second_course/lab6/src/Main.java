
public class Main {

    public static void main(String[] args) {
        WordStat stat = new WordStat();
        String inputPath, outputPath;

        inputPath = new String();
        outputPath = new String();

        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-i")) {
                if (args.length-1 != i) {
                    inputPath = args[i+1];
                    i++;
                }
            }
            else if (args[i].equals("-o")) {
                if (args.length-1 != i) {
                    outputPath = args[i+1];
                    i++;
                }
            }
            else {
                System.err.println("Unknown argument \"" + args[i] + "\".");
            }
        }

        if (inputPath.isEmpty()) {
            System.err.println("Input file isn't specified.");
        }
        if (outputPath.isEmpty()) {
            System.err.println("Output file isn't specified.");
            outputPath = "out.csv";
        }

        try {
            stat.complete(inputPath, outputPath, ".,/'\\\";:[]{}!@#$%^&*()-=_+<> ");
        }
        catch (RuntimeException e) {
            System.err.println(e.getMessage());
        }
    }
}