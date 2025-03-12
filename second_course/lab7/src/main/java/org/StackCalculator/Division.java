package org.StackCalculator;

import org.Exceptions.OperationException;

import java.util.ArrayList;

public final class Division extends Operation {

    @Override
    public void complete(Context context, ArrayList<String> args) throws OperationException {
        double d1, d2;

        if (context.stack().size() < 2) {
            throw new OperationException("Stack doesn't contain two elements.");
        }

        d1 = context.stack().pop();
        d2 = context.stack().pop();

        if (d2 == 0.0f) {
            context.stack().push(d2);
            context.stack().push(d1);
            throw new OperationException("Division by zero.");
        }
        else {
            context.stack().push(d1 / d2);
        }
    }
}
