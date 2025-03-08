package org.StackCalculator;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Stack;
import java.util.TreeMap;

import java.util.logging.Level;
import java.util.logging.Logger;

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

        Logger logger = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

        try {

            Factory factory = new Factory();
            Context context = new Context(new Stack<Double>(), new TreeMap<String, Double>());
            ConfigParser configParser = null;

            if (args.length == 0) {
                logger.log(Level.INFO, "Input path isn't specified");
                logger.log(Level.INFO, "Opening \"config.txt\" as input file.");
                configParser = new ConfigParser("config.txt");
            }
            else {
                for (int i = 0; i < args.length && configParser == null; i++) {
                    if (args[i].equals("-i") && i < args.length - 1) {
                        logger.log(Level.INFO, "Opening \"" + args[i + 1] + "\" as input file.");
                        configParser = new ConfigParser(args[i + 1]);

                    }
                    else {
                        logger.log(Level.WARNING, "Flag -i is given, but input path isn't specified");
                        logger.log(Level.INFO, "Opening \"config.txt\" as input file.");
                        configParser = new ConfigParser("config.txt");
                    }
                }
            }

            String[] strs;
            String op;
            ArrayList<String> tmp;

            strs = configParser.getCommands();

            for (int i = 0; i < strs.length; i++) {
                logger.log(Level.INFO, "Working with string " + i);

                if (strs[i].charAt(0) != '#' && !strs[i].isEmpty()) {
                    tmp = tokenize(strs[i]);
                    op = tmp.get(0);
                    tmp.remove(0);
                    try {
                        logger.log(Level.INFO, "Trying to create operation " + op);
                        factory.createOperation(strs[i], i).complete(context, tmp);
                        logger.log(Level.FINEST, "Completed operation " + op);
                    }
                    catch (OperationException | StackException ex) {
                        logger.log(Level.WARNING, "Failed to complete operation \"" + op + "\" in line: " + i + " \n Reason: " + ex.getMessage());
                        System.out.println(ex.getMessage());
                    }
                }
            }
        }
        catch (FactoryException | IOException e) {
            logger.log(Level.SEVERE, "Failed to initialise, reason: " + e.getMessage());
            System.out.println(e.getClass() + " " + e.getMessage());
        }
    }
}
