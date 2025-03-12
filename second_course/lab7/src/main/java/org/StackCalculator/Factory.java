package org.StackCalculator;

import org.Exceptions.FactoryException;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.TreeMap;

public final class Factory {

    TreeMap<String, String> commandMap;

    public Factory() throws FactoryException {
        BufferedReader cfgReader;
        String[] strs;

        try {
            cfgReader = new BufferedReader(new FileReader("src/main/java/org/StackCalculator/FactoryConfig.txt"));
            this.commandMap = new TreeMap<String, String>();

            strs = cfgReader.lines().toList().toArray(new String[0]);

            for (String str: strs) {
                this.commandMap.put(Main.tokenize(str).get(0), Main.tokenize(str).get(1));
            }
        }
        catch (FileNotFoundException e) {
            throw new FactoryException("Factory config file not found.");
        }
        catch (IndexOutOfBoundsException e) {
            throw new FactoryException("Factory config is incorrect.");
        }
    }

     public Operation createOperation(String command, int commandLine) throws FactoryException {
        ArrayList<String> tokens;
        Class<?> operation;

        tokens = Main.tokenize(command);

        if (!commandMap.containsKey(tokens.get(0))) {
            throw new FactoryException("Unknown command \"" + tokens.get(0) +  "\" in line " + commandLine + ".");
        }
        else {
            try {
                operation = Class.forName(commandMap.get(tokens.get(0)));
                return (Operation) operation.getDeclaredConstructor().newInstance();
            }
            catch (ClassNotFoundException e) {
                throw new FactoryException("Class \"" + commandMap.get(tokens.get(0)) + "\" is not found.");
            }
            catch (InstantiationException e) {
                throw new FactoryException("Error initializing class \"" + commandMap.get(tokens.get(0))  + "\".");
            }
            catch (IllegalAccessException e) {
                throw new FactoryException("Illegal access in class \"" + commandMap.get(tokens.get(0)) + "\".");
            }
            catch (NoSuchMethodException e) {
                throw new FactoryException("No constructor for class \"" + commandMap.get(tokens.get(0)) + "\".");
            }
            catch (InvocationTargetException e) {
                throw new FactoryException("\"" + commandMap.get(tokens.get(0)) + "\" had thrown an exception on invocation.");
            }
        }
     }
}
