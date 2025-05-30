package org.workers;

import org.factory.Accessory;
import org.factory.Body;
import org.factory.Car;
import org.factory.Engine;
import org.storages.Storage;
import org.threadpool.Task;

public class AssemblyTask implements Task {
    private final Storage<Body> bodyStorage;
    private final Storage<Engine> engineStorage;
    private final Storage<Accessory> accessoryStorage;
    private final Storage<Car> carStorage;

    public AssemblyTask(Storage<Body> bodyStorage, Storage<Engine> engineStorage,
                        Storage<Accessory> accessoryStorage, Storage<Car> carStorage) {
        this.bodyStorage = bodyStorage;
        this.engineStorage = engineStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
    }

    @Override
    public void execute() {
        try {
            Body body = this.bodyStorage.take();
            Engine engine = this.engineStorage.take();
            Accessory accessory = this.accessoryStorage.take();
            Car car = new Car(body, engine, accessory);
            this.carStorage.put(car);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}