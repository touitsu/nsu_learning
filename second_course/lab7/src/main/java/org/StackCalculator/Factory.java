package org.StackCalculator;

import org.Exceptions.FactoryConfigAbsentException;
import org.Exceptions.FactoryConfigFormatException;
import org.Exceptions.FactoryException;
import org.Exceptions.UnknownCommandException;
import org.Operations.Operation;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.TreeMap;

public final class Factory {

    TreeMap<String, String> commandMap;

    public Factory() throws FactoryException {
        BufferedReader cfgReader;
        String[] strs;

        try (BufferedReader file = new BufferedReader(new FileReader("src/main/java/org/StackCalculator/FactoryConfig.txt"))) {

            this.commandMap = new TreeMap<String, String>();

            for (String line = file.readLine(); line != null; line = file.readLine()) {
                this.commandMap.put(line.substring(0, line.indexOf(' ')), line.substring(line.indexOf(' ') + 1));
            }
        }
        catch (FileNotFoundException e) {
            throw new FactoryConfigAbsentException(e.getMessage());
        }
        catch (IOException e) {
            throw new FactoryConfigFormatException(e.getMessage());
        }
    }

     public Operation createOperation(String command, int commandLine) throws FactoryException {
        Class<?> operation;

        if (!commandMap.containsKey(command)) {
            throw new UnknownCommandException("Unknown command \"" + command +  "\" in line " + commandLine + ".");
        }
        else {
            try {
                operation = Class.forName(commandMap.get(command));
                return (Operation) operation.getDeclaredConstructor().newInstance();
            }
            catch (ClassNotFoundException e) {
                throw new org.Exceptions.ClassNotFoundException("Class \"" + commandMap.get(command) + "\" is not found.");
            }
            catch (InstantiationException e) {
                throw new FactoryException("Error initializing class \"" + commandMap.get(command)  + "\".");
            }
            catch (IllegalAccessException e) {
                throw new FactoryException("Illegal access in class \"" + commandMap.get(command) + "\".");
            }
            catch (NoSuchMethodException e) {
                throw new FactoryException("No constructor for class \"" + commandMap.get(command) + "\".");
            }
            catch (InvocationTargetException e) {
                throw new FactoryException("\"" + commandMap.get(command) + "\" had thrown an exception on invocation.");
            }
        }
     }
}
