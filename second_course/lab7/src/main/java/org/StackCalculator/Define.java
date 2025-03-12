package org.StackCalculator;

import org.Exceptions.OperationException;

import java.util.ArrayList;

public final class Define extends Operation {

    public void complete(Context context, ArrayList<String> args) throws OperationException {
        double tmp;

        if (args.size() != 2) {
            throw new OperationException("Wrong number of arguments for a define call (expected 2, but got " + args.size() + " instead).");
        }

        try {
            Double.parseDouble(args.get(0));
            throw new OperationException("Variable name contains a parsable double, therefore it can not be a variable.");
        }
        catch (NumberFormatException e) {
            try {
                tmp = Double.parseDouble(args.get(1));
                context.map().put(args.get(0), tmp);
            }
            catch (NumberFormatException ex) {
                throw new OperationException("Variable value is not a parsable double.");
            }
        }
    }
}
