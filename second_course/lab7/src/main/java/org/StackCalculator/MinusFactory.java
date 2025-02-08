package org.StackCalculator;

public final class MinusFactory extends OperationFactory {

    @Override
    Operation createOperation(Context context) {
        return new Minus(context);
    }
}
