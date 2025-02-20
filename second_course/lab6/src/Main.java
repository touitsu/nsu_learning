import java.util.ArrayList;
import java.util.TreeSet;

public class Main {

    private static int ArrayIdx(ArrayList<Word> wordList, String str) {
        for (int i = 0; i < wordList.size(); i++) {
            if (wordList.get(i).toString().equals(str)) {
                return i;
            }
        }

        return -1;
    }

    public static void main(String[] args) {
        Parser parser;
        ArrayList<String> tokenList;
        ArrayList<Word> wordList;
        TreeSet<Word> set;
        CsvDumper dumper;
        final char[] sep = " ,.<>[](){}!@#$%^&*_+-'\"\\/|?;:=".toCharArray();

        try {

            parser = new Parser("input.txt");

            for (int i = 0; i < args.length; i++) {
                if (args[i].equals("-i")) {
                    try {
                        parser = new Parser(args[i+1]);
                    }
                    catch (ArrayIndexOutOfBoundsException e) {
                        System.out.println("Flag -i is given, but input path is not specified.");
                    }
                }
            }

            if (parser.getPath().equals("input.txt")) {
                System.out.println("Input path is not specified.");
            }

            dumper = new CsvDumper("out.csv");

            for (int i = 0; i < args.length; i++) {
                if (args[i].equals("-o")) {
                    try {
                        dumper = new CsvDumper(args[i+1]);
                    }
                    catch (ArrayIndexOutOfBoundsException e) {
                        System.out.println("Flag -o is given, but output path is not specified.");
                    }
                }
            }

            if (dumper.getPath().equals("out.csv")) {
                System.out.println("Output path is not specified.");
            }

            wordList = new ArrayList<Word>();
            tokenList = parser.tokenize(sep);

            for (String s : tokenList) {

                int idx = ArrayIdx(wordList, s);

                if (idx != -1) {
                    wordList.get(idx).incFreq();
                } else {
                    Word newWord = new Word(s);
                    wordList.add(newWord);
                }
            }

            set = new TreeSet<Word>(wordList);

            Word.setTotalWords(set.size());

            dumper.dump(set);
        }
        catch (RuntimeException e) {
            System.out.println(e.getMessage());
        }
    }
}