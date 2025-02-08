package org.StackCalculator;

public final class SqrtFactory extends OperationFactory {

    @Override
    Operation createOperation(Context context) {
        return new Sqrt(context);
    }
}
