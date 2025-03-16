package org.StackCalculator;

import org.Exceptions.*;

import java.util.ArrayList;
import java.util.Stack;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;

public final class Calculator {
    private final Logger logger;
    private final Factory factory;
    private final Context context;
    private ConfigParser configParser;

    private ArrayList<String> tokenize(String line) {
        ArrayList<String> res;

        if (line == null) {
            return null;
        }

        res = new ArrayList<String>();

        for (int i = 0, lastIdx = 0; i < line.length(); i++) {
            if (line.charAt(i) == ' ' || i == line.length() - 1) {
                int curI = i + (i == line.length() - 1 ? 1 : 0);
                if (curI != lastIdx) {
                    res.add(line.substring(lastIdx, curI));
                }
                lastIdx = i + 1;
            }
        }

        return res;
    }

    public Calculator(String[] args, Logger logger) throws CalculatorException {
        this.logger = logger;

        try {
            this.factory = new Factory();
        }
        catch (FactoryException e) {
            throw new CalculatorException("Can't create a factory, reason: \"" + e.getMessage() + "\".");
        }

        this.context = new Context(new Stack<Double>(), new TreeMap<String, Double>());
        this.configParser = null;

        if (args.length == 0) {
            this.logger.log(Level.INFO, "Input path isn't specified.");
        }
        else {
            for (int i = 0; i < args.length && this.configParser == null; i++) {
                if (args[i].equals("-i") && i < args.length - 1) {
                    this.logger.log(Level.INFO, "Opening \"" + args[i + 1] + "\" as input file.");
                    this.configParser = new ConfigParser(args[i + 1]);

                }
                else {
                    this.logger.log(Level.WARNING, "Flag -i is given, but input path isn't specified");

                }
            }
        }

        if (this.configParser == null) {
            this.logger.log(Level.INFO, "Using default input stream as input file.");
            configParser = new ConfigParser("Stream");
        }
    }

    public void complete() {
        try {
            ArrayList<String> tokens;

            tokens = tokenize(configParser.getLine());

            for (int opIdx = 0; tokens != null; opIdx++, tokens = tokenize(configParser.getLine())) {
                logger.log(Level.INFO, "Working with string " + opIdx);

                if (tokens.get(0).charAt(0) != '#') {
                    try {
                        logger.log(Level.INFO, "Trying to create operation " + tokens.get(0));
                        factory.createOperation(tokens.get(0), opIdx).complete(context, new ArrayList<String>(tokens.subList(1, tokens.size())));
                        logger.log(Level.INFO, "Completed operation " + tokens.get(0));
                    }
                    catch (OperationException | FactoryException | StackException ex) {
                        logger.log(Level.WARNING, "Failed to complete operation \"" + tokens.get(0) + "\" in line: " + opIdx + " \n Reason: " + ex.getMessage());
                    }
                }
            }
        }
        catch (ConfigException e) {
            logger.log(Level.SEVERE, "Error reading tokens, reason: " + e.getMessage());
        }
    }
}
