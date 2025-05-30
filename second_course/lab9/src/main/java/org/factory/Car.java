package org.factory;

public class Car {
    private static int nextId = 1;
    private final int id;
    private final Body body;
    private final Engine engine;
    private final Accessory accessory;

    public Car(Body body, Engine engine, Accessory accessory) {
        this.id = nextId++;
        this.body = body;
        this.engine = engine;
        this.accessory = accessory;
    }

    public int getId() {
        return this.id;
    }

    public Body getBody() {
        return this.body;
    }

    public Engine getEngine() {
        return this.engine;
    }

    public Accessory getAccessory() {
        return this.accessory;
    }
}