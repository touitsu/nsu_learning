package org.StackCalculator;

import java.util.EmptyStackException;
import java.lang.Math;

public final class Sqrt extends Operation {

    public Sqrt(Context context) {
        super(context);
    }

    @Override
    public void complete() throws RuntimeException {
        Double d1;

        try {
            d1 = this.context.stack().pop();
            this.context.stack().push(Math.pow(d1, 0.5));
        }
        catch (EmptyStackException e) {
            throw new RuntimeException("The stack is empty.");
        }
    }
}
