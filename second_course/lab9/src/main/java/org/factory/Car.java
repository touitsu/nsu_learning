package org.factory;

import java.util.concurrent.atomic.AtomicInteger;

public class Car {
    private static AtomicInteger nextId = new AtomicInteger(1);
    private final int id;
    private final Body body;
    private final Engine engine;
    private final Accessory accessory;

    public Car(Body body, Engine engine, Accessory accessory) {
        this.id = nextId.addAndGet(1);
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