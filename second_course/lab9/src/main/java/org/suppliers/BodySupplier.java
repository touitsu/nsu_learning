package org.suppliers;

import org.factory.Body;
import org.storages.Storage;

import java.util.concurrent.atomic.AtomicInteger;

public class BodySupplier extends Supplier<Body> {
    private static AtomicInteger nextId = new AtomicInteger(1);

    public BodySupplier(Storage<Body> storage, int delay) {
        super(storage, delay);
    }

    @Override
    protected Body createPart() {
        return new Body(nextId.addAndGet(1));
    }
}