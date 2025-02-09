package org.StackCalculator;

import java.util.ArrayList;

public final class Push extends Operation {

    public void complete(Context context, ArrayList<String> args) throws OperationException {
        double tmp;

        if (args.size() != 1) {
            throw new OperationException("Wrong number of arguments in push call (expected 1, but got " + args.size() + " instead).");
        }

        try {
            tmp = Double.parseDouble(args.get(0));

            context.stack().push(tmp);
        }
        catch (NumberFormatException e) {
            if (context.map().get(args.get(0)) == null) {
                throw new OperationException("Variable \"" +  args.get(0) + "\" is not defined.");
            }

            context.stack().push(context.map().get(args.get(0)));
        }
    }
}
