package org.StackCalculator;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.TreeMap;

public class Factory {

    TreeMap<String, String> commandMap;

    public Factory() throws IOException {
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
            throw new IOException("Factory config file not found.");
        }
        catch (IndexOutOfBoundsException e) {
            throw new IOException("Factory config is incorrect.");
        }
    }

     Operation createOperation(String command, int commandLine) throws IOException {
        ArrayList<String> tokens;
        Class<?> operation;

        tokens = Main.tokenize(command);

        if (!commandMap.containsKey(tokens.get(0))) {
            throw new IOException("Unknown command \"" + tokens.get(0) +  "\" in line " + commandLine + ".");
        }
        else {
            try {
                operation = Class.forName(commandMap.get(tokens.get(0)));
                return (Operation) operation.getDeclaredConstructor().newInstance();
            }
            catch (ClassNotFoundException | InvocationTargetException e) {
                throw new IOException("Class \"" + commandMap.get(tokens.get(0)) + "\" is not found.");
            }
            catch (InstantiationException e) {
                throw new IOException("Error initializing class \"" + commandMap.get(tokens.get(0))  + "\".");
            }
            catch (IllegalAccessException e) {
                throw new IOException("Illegal access in class \"" + commandMap.get(tokens.get(0)) + "\".");
            }
            catch (NoSuchMethodException e) {
                throw new IOException("No constructor for class \"" + commandMap.get(0) + "\".");
            }
        }
     }

}
