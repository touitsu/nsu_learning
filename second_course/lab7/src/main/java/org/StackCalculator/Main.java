package org.StackCalculator;

import java.util.ArrayList;
import java.util.Arrays;
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
        StackCalculator stackCalculator = new StackCalculator();
        ConfigParser configParser = new ConfigParser("config.txt");
        CommandHandler commandHandler = new CommandHandler(new Context(stackCalculator, new TreeMap<>()));
        String[] strs;

        try {
            strs = configParser.getCommands();

            for (int i = 0; i < strs.length; i++) {
                if (strs[i].charAt(0) != '#') {
                    commandHandler.handleCommand(tokenize(strs[i]), i + 1);
                }
            }
        }
        catch (RuntimeException e) {
            System.out.println(e.getMessage());
        }
    }
}
