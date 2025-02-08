package org.StackCalculator;

public final class MultiplyFactory extends OperationFactory {

    @Override
    Operation createOperation(Context context) {
        return new Multiply(context);
    }
}
