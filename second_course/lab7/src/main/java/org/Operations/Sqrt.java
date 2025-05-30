package org.Operations;

import org.Exceptions.StackException;
import org.Exceptions.StackUnderflowException;
import org.StackCalculator.Context;

import java.util.ArrayList;
import java.lang.Math;

public final class Sqrt extends Operation {

    @Override
    public void complete(Context context, ArrayList<String> args) throws StackException {
        double d1;

        if (context.stack().isEmpty()) {
            throw new StackUnderflowException("The stack is empty.");
        }

        d1 = context.stack().pop();
        context.stack().push(Math.pow(d1, 0.5));
    }
}
