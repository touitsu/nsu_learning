package org.Operations;

import org.Exceptions.StackException;
import org.Exceptions.StackUnderflowException;
import org.StackCalculator.Context;

import java.util.ArrayList;

public final class Print extends Operation {

    public void complete(Context context, ArrayList<String> args) throws StackException {
        double d1;

        if (context.stack().isEmpty()) {
            throw new StackUnderflowException("Stack is empty.");
        }

        d1 = context.stack().pop();

        System.out.println(d1);

        context.stack().push(d1);
    }
}
