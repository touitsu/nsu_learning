package org.suppliers;

import org.factory.Engine;
import org.storages.Storage;

import java.util.concurrent.atomic.AtomicInteger;

public class EngineSupplier extends Supplier<Engine> {
    private static AtomicInteger nextId = new AtomicInteger(1);

    public EngineSupplier(Storage<Engine> storage, int delay) {
        super(storage, delay);
    }

    @Override
    protected Engine createPart() {
        return new Engine(nextId.addAndGet(1));
    }
}