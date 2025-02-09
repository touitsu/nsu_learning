package org.StackCalculator;

import java.util.Stack;
import java.util.TreeMap;

public record Context(Stack<Double> stack, TreeMap<String, Double> map) {

}
