package org.controller;

import org.factory.Accessory;
import org.factory.Body;
import org.factory.Engine;
import org.storages.*;
import org.threadpool.ThreadPool;
import org.workers.AssemblyTask;

public class ProductionController implements Runnable {
    private final CarStorage carStorage;
    private final ThreadPool threadPool;
    private final Storage<Body> bodyStorage;
    private final Storage<Engine> engineStorage;
    private final Storage<Accessory> accessoryStorage;
    private final int targetStockLevel;

    public ProductionController(CarStorage carStorage, ThreadPool threadPool,
                                Storage<Body> bodyStorage, Storage<Engine> engineStorage,
                                Storage<Accessory> accessoryStorage) {
        this.carStorage = carStorage;
        this.threadPool = threadPool;
        this.bodyStorage = bodyStorage;
        this.engineStorage = engineStorage;
        this.accessoryStorage = accessoryStorage;
        this.targetStockLevel = carStorage.getCapacity() / 2;
    }

    @Override
    public void run() {
        try {
            while (!Thread.currentThread().isInterrupted()) {
                synchronized (carStorage) {
                    while (carStorage.size() >= targetStockLevel) {
                        carStorage.wait();
                    }
                }

                threadPool.addTask(new AssemblyTask(
                        bodyStorage, engineStorage, accessoryStorage, carStorage
                ));

                Thread.sleep(10);
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}