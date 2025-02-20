package org.StackCalculator;

import java.util.ArrayList;
import java.util.EmptyStackException;

public final class Pop extends Operation {

    public void complete(Context context, ArrayList<String> args) throws StackException {
        try {
            context.stack().pop();
        }
        catch (EmptyStackException e) {
            throw new StackException("Stack is empty.");
        }
    }
}
