package org.StackCalculator;

public final class DivideFactory extends OperationFactory {

    @Override
    Operation createOperation(Context context) {
        return new Divide(context);
    }
}
