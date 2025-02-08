package org.StackCalculator;

public abstract class Operation {
    protected Context context;

    public Operation(Context context) {
        this.context = context;
    }

    public abstract void complete() throws RuntimeException;
}
