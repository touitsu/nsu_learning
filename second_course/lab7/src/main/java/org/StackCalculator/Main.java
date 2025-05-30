package org.StackCalculator;

import org.Exceptions.CalculatorException;

import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {

    public static void main(String[] args) {
        Logger logger = Logger.getLogger(Logger.GLOBAL_LOGGER_NAME);

        try {
            Calculator calculator = new Calculator(args, logger);
            calculator.complete();
        }
        catch (CalculatorException e) {
            logger.log(Level.SEVERE, "Can't initialize calculator, reason: \"" + e.getMessage() + "\".");
        }
    }
}
