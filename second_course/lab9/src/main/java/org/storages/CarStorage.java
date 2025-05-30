package org.storages;

import org.factory.Car;

public class CarStorage extends Storage<Car> {
    public CarStorage(int capacity) {
        super(capacity);
    }
}