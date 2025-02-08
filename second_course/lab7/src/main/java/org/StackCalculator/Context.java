package org.StackCalculator;

import java.util.TreeMap;

public record Context(StackCalculator stack, TreeMap<String, Double> map) {

}
