package org.Operations;

import org.Exceptions.StackException;
import org.Exceptions.StackUnderflowException;
import org.StackCalculator.Context;

import java.util.ArrayList;
import java.util.EmptyStackException;

public final class Pop extends Operation {

    public void complete(Context context, ArrayList<String> args) throws StackException {
        try {
            context.stack().pop();
        }
        catch (EmptyStackException e) {
            throw new StackUnderflowException("Stack is empty.");
        }
    }
}
