package org.StackCalculator;

public final class PlusFactory extends OperationFactory {

    @Override
    Operation createOperation(Context context) {
        return new Plus(context);
    }
}
