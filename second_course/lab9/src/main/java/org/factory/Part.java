package org.factory;

public abstract class Part {
    private final int id;

    protected Part(int id) {
        this.id = id;
    }

    public int getId() {
        return this.id;
    }
}