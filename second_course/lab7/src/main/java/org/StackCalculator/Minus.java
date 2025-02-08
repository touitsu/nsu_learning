package org.StackCalculator;

import java.util.EmptyStackException;

public final class Minus extends Operation {

    public Minus(Context context) {
        super(context);
    }

    @Override
    public void complete() throws RuntimeException {
        Double d1, d2;

        try {
            d1 = this.context.stack().pop();
            d2 = this.context.stack().pop();
            this.context.stack().push(d1 - d2);
        }
        catch (EmptyStackException e) {
            throw new RuntimeException("The stack does not have two elements.");
        }
    }
}
