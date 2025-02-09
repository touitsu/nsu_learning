package org.StackCalculator;

import java.util.ArrayList;
import java.util.EmptyStackException;

public final class Plus extends Operation {

    @Override
    public void complete(Context context, ArrayList<String> args) throws OperationException {
        Double d1, d2;

        d1 = Double.NaN;

        try {
            d1 = context.stack().pop();
            d2 = context.stack().pop();
            context.stack().push(d1 + d2);
        }
        catch (EmptyStackException e) {
            if (!d1.isNaN()) {
                context.stack().push(d1);
            }
            throw new OperationException("The stack does not have two elements.");
        }
    }
}
