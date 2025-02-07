public final class Word implements Comparable<Word>{
    private final String content;
    private Integer freq;
    private static Integer totalWords;

    public Word(String str) {
        this.content = str;
        this.freq = 1;
    }

    public void incFreq() {
        this.freq++;
    }

    public Integer getFreq() {
        return this.freq;
    }

    public static void setTotalWords(Integer totalWords) {
        Word.totalWords = totalWords;
    }

    public Double getPercentage() {
        return (double) this.freq / Word.totalWords;
    }

    @Override
    public String toString() {
        return content;
    }

    @Override
    public int compareTo(Word w) {
        if (this.content.equals(w.content)) {
            return 0;
        }
        else if (this.freq >= w.freq) {
            return 1;
        }
        else {
            return -1;
        }
    }
}