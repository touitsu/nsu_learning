package org.suppliers;

import org.factory.Car;
import org.storages.CarStorage;
import org.util.Logger;

import java.util.concurrent.atomic.AtomicInteger;

public class Dealer implements Runnable {
    private final CarStorage storage;
    private final int id;
    private volatile int delay;
    private static final AtomicInteger carsSold = new AtomicInteger();

    public Dealer(CarStorage storage, int id, int delay) {
        this.storage = storage;
        this.id = id;
        this.delay = delay;
    }

    public void setDelay(int delay) {
        this.delay = delay;
    }

    public static int getCarsSold() {
        return carsSold.get();
    }

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                Thread.sleep(this.delay);
                Car car = this.storage.take();
                carsSold.incrementAndGet();
                Logger.logSale(this.id, car);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}