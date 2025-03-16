package org.Operations;

import org.Exceptions.NotEnoughArgumentsException;
import org.Exceptions.OperationException;
import org.Exceptions.UnknownVariableException;
import org.StackCalculator.Context;

import java.util.ArrayList;

public final class Push extends Operation {

    public void complete(Context context, ArrayList<String> args) throws OperationException {
        double tmp;

        if (args.size() != 1) {
            throw new NotEnoughArgumentsException("Wrong number of arguments in push call (expected 1, but got " + args.size() + " instead).");
        }

        try {
            tmp = Double.parseDouble(args.get(0));

            context.stack().push(tmp);
        }
        catch (NumberFormatException e) {
            if (context.map().get(args.get(0)) == null) {
                throw new UnknownVariableException("Variable \"" +  args.get(0) + "\" is not defined.");
            }

            context.stack().push(context.map().get(args.get(0)));
        }
    }
}
