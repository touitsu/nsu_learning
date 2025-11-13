package org.example;

public class Place {
    public String xid;
    public String name;
    public double dist;
    public String description = "";

    @Override
    public String toString() {
        return name + " (" + String.format("%.0f м", dist) + ")\n" +
                (description.isEmpty() ? "— Without description —" : description);
    }

    public String name() {
        return this.name;
    }
}