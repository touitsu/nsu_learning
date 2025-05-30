package org.storages;

import java.util.LinkedList;
import java.util.Queue;

public class Storage<T> {
    private final Queue<T> items = new LinkedList<>();
    private final int capacity;
    private int produced = 0;

    public Storage(int capacity) {
        this.capacity = capacity;
    }

    public synchronized void put(T item) throws InterruptedException {
        while (this.items.size() >= this.capacity) {
            wait();
        }
        this.items.add(item);
        this.produced++;
        notifyAll();
    }

    public synchronized T take() throws InterruptedException {
        while (this.items.isEmpty()) {
            wait();
        }
        T item = this.items.poll();
        notifyAll();
        return item;
    }

    public synchronized int size() {
        return this.items.size();
    }

    public synchronized int getProduced() {
        return this.produced;
    }

    public int getCapacity() {
        return this.capacity;
    }
}