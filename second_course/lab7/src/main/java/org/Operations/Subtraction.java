package org.Operations;

import org.Exceptions.StackException;
import org.Exceptions.StackUnderflowException;
import org.StackCalculator.Context;

import java.util.ArrayList;

public final class Subtraction extends Operation {

    @Override
    public void complete(Context context, ArrayList<String> args) throws StackException {
        double d1, d2;

        if (context.stack().size() < 2) {
            throw new StackUnderflowException("Stack doesn't contain two elements.");
        }

        d1 = context.stack().pop();
        d2 = context.stack().pop();

        context.stack().push(d1 - d2);
    }
}
