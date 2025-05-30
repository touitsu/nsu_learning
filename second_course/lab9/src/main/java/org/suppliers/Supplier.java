package org.suppliers;

import org.storages.Storage;

public abstract class Supplier<T> implements Runnable {
    protected final Storage<T> storage;
    protected volatile int delay;

    public Supplier(Storage<T> storage, int delay) {
        this.storage = storage;
        this.delay = delay;
    }

    public void setDelay(int delay) {
        this.delay = delay;
    }

    protected abstract T createPart();

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                Thread.sleep(delay);
                T part = createPart();
                this.storage.put(part);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}