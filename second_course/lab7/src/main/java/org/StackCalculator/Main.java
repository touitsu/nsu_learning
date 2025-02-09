package org.StackCalculator;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Stack;
import java.util.TreeMap;

public class Main {
    public static ArrayList<String> tokenize(String str) {
        ArrayList<String> res;
        int lidx;

        res = new ArrayList<String>();
        lidx = 0;

        for (int i = 0; i < str.length(); i++) {
            if (str.charAt(i) == ' ') {
                if (lidx != i) {
                    res.add(str.substring(lidx, i));
                }

                lidx = i + 1;
            }
        }

        if (lidx < str.length()) {
            res.add(str.substring(lidx));
        }

        return res;
    }

    public static void main(String[] args) {
        try {

            Factory factory = new Factory();
            Context context = new Context(new Stack<Double>(), new TreeMap<String, Double>());
            ConfigParser configParser = new ConfigParser("config.txt");
            String[] strs;
            ArrayList<String> tmp;

            strs = configParser.getCommands();

            for (int i = 0; i < strs.length; i++) {
                if (strs[i].charAt(0) != '#' && !strs[i].isEmpty()) {
                    tmp = tokenize(strs[i]);
                    tmp.remove(0);
                    try {
                        factory.createOperation(strs[i], i).complete(context, tmp);
                    }
                    catch (OperationException ex) {
                        System.out.println(ex.getMessage());
                    }
                }
            }
        }
        catch (IOException e) {
            System.out.println(e.getMessage());
        }
    }
}
