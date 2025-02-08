package org.StackCalculator;

import java.util.ArrayList;

public final class CommandHandler {
    private Context context;

    public CommandHandler(Context context) {
        this.context = context;
    }

    public void handleCommand(ArrayList<String> command, int lineNumber) throws RuntimeException {
        if (command.isEmpty()) {
            throw new RuntimeException("Command is empty.");
        }
        else {
            switch (command.get(0)) {
                case "DEFINE" : {
                    if (command.size() != 3) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (must be two).");
                    }

                    try {
                        Double.parseDouble(command.get(1));
                        throw new RuntimeException("Variable \"" + command.get(1) +  "\" in line " + lineNumber + " contains a parsable double, therefore it can not be considered a variable.");
                    }
                    catch (NumberFormatException e) {
                        this.context.map().put(command.get(1), Double.parseDouble(command.get(2)));
                    }

                    break;
                }
                case "PRINT" : {
                    double tmp;

                    tmp = this.context.stack().pop();
                    System.out.println(tmp);
                    this.context.stack().push(tmp);

                    break;
                }
                case "POP" : {
                    if (command.size() != 1) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (command does not support arguments).");
                    }
                    this.context.stack().pop();
                    break;
                }
                case "PUSH" : {
                    if (command.size() != 2) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (must be one).");
                    }
                    double tmp;

                    try {
                        tmp = Double.parseDouble(command.get(1));
                        this.context.stack().push(tmp);
                    }
                    catch (NumberFormatException e) {
                        if(this.context.map().containsKey(command.get(1))) {
                            this.context.stack().push(this.context.map().get(command.get(1)));
                        }
                        else {
                            throw new RuntimeException("Unknown variable " + command.get(1) + " in line " + lineNumber + ".");
                        }
                    }

                    break;
                }
                case "+": {
                    if (command.size() != 1) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (command does not support arguments).");
                    }
                    new PlusFactory().createOperation(this.context).complete();
                    break;
                }
                case "-": {
                    if (command.size() != 1) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (command does not support arguments).");
                    }
                    new MinusFactory().createOperation(this.context).complete();
                    break;
                }
                case "*": {
                    if (command.size() != 1) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (command does not support arguments).");
                    }
                    new MultiplyFactory().createOperation(this.context).complete();
                    break;
                }
                case "/": {
                    if (command.size() != 1) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (command does not support arguments).");
                    }
                    new DivideFactory().createOperation(this.context).complete();
                    break;
                }
                case "SQRT": {
                    if (command.size() != 1) {
                        throw new RuntimeException("Invalid amount of arguments in line " + lineNumber + " (command does not support arguments).");
                    }
                    new SqrtFactory().createOperation(this.context).complete();
                    break;
                }
                default: {
                    throw new RuntimeException("Unknow command in line " + lineNumber + ".");
                }
            }
        }
    }
}
