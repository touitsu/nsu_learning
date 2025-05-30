package org.suppliers;

import org.factory.Accessory;
import org.storages.Storage;

public class AccessorySupplier extends Supplier<Accessory> {
    private static int nextId = 1;

    public AccessorySupplier(Storage<Accessory> storage, int delay) {
        super(storage, delay);
    }

    @Override
    protected Accessory createPart() {
        return new Accessory(nextId++);
    }
}