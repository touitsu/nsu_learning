package org.StackCalculator;

import java.util.EmptyStackException;
import java.util.Stack;
import java.util.TreeMap;
import java.lang.Math;

public final class StackCalculator extends Stack<Double> {
    private TreeMap<String, Double> map;

    public StackCalculator() {
        this.map = new TreeMap<String, Double>();
    }

    public void print() throws RuntimeException {
        Double tmp;
        try {
            tmp = this.pop();

            System.out.println(tmp);

            this.push(tmp);
        }
        catch (EmptyStackException e) {
            throw new RuntimeException("Trying to print element from an empty stack.");
        }
    }

    public void define (String str, Double val) throws RuntimeException {
        if (str.isEmpty()) {
            throw new RuntimeException("Trying to define a value of an empty string.");
        }
        else {
            this.map.put(str, val);
        }
    }

    public double getDefinedVal(String key) throws RuntimeException {
        if (map.containsKey(key)) {
            throw new RuntimeException("Value of " + key + " is not initialized.");
        }
        else {
            return map.get(key);
        }
    }
}
