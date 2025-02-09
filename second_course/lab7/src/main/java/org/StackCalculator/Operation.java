package org.StackCalculator;

import java.util.ArrayList;

public abstract class Operation {

    public abstract void complete(Context context, ArrayList<String> args) throws OperationException;
}
