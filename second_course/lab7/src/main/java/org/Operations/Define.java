package org.Operations;

import org.Exceptions.BadNameException;
import org.Exceptions.NotEnoughArgumentsException;
import org.Exceptions.OperationException;
import org.Exceptions.ValueNaNException;
import org.StackCalculator.Context;

import java.util.ArrayList;

public final class Define extends Operation {

    @Override
    public void complete(Context context, ArrayList<String> args) throws OperationException {
        double tmp;

        if (args.size() != 2) {
            throw new NotEnoughArgumentsException("Wrong number of arguments for a define call (expected 2, but got " + args.size() + " instead).");
        }

        try {
            Double.parseDouble(args.get(0));
            throw new BadNameException("Variable name contains a parsable double, therefore it can not be a variable.");
        }
        catch (NumberFormatException e) {
            try {
                tmp = Double.parseDouble(args.get(1));
                context.map().put(args.get(0), tmp);
            }
            catch (NumberFormatException ex) {
                throw new ValueNaNException("Variable value is not a parsable double.");
            }
        }
    }
}
