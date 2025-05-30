package org.suppliers;

import org.factory.Body;
import org.storages.Storage;

public class BodySupplier extends Supplier<Body> {
    private static int nextId = 1;

    public BodySupplier(Storage<Body> storage, int delay) {
        super(storage, delay);
    }

    @Override
    protected Body createPart() {
        return new Body(nextId++);
    }
}