package org.StackCalculator;

import org.Exceptions.OperationException;
import org.Exceptions.StackException;

import java.util.ArrayList;

public abstract class Operation {

    public abstract void complete(Context context, ArrayList<String> args) throws OperationException, StackException;
}
