package org.StackCalculator;

import java.util.ArrayList;
import java.util.EmptyStackException;

public final class Print extends Operation {

    public void complete(Context context, ArrayList<String> args) throws OperationException {
        double tmp;

        try {
            tmp = context.stack().pop();

            System.out.println(tmp);

            context.stack().push(tmp);
        }
        catch (EmptyStackException e) {
            throw new OperationException("The stack is empty.");
        }
    }
}
