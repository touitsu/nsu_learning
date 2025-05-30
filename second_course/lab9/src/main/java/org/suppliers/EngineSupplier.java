package org.suppliers;

import org.factory.Engine;
import org.storages.Storage;

public class EngineSupplier extends Supplier<Engine> {
    private static int nextId = 1;

    public EngineSupplier(Storage<Engine> storage, int delay) {
        super(storage, delay);
    }

    @Override
    protected Engine createPart() {
        return new Engine(nextId++);
    }
}