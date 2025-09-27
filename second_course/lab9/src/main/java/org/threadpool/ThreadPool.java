package org.threadpool;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class ThreadPool {
    private final BlockingQueue<Task> taskQueue;
    private final Thread[] workers;
    private volatile boolean isRunning = true;
    private int completedTasks = 0;

    public ThreadPool(int size, int queueCapacity) {
        this.taskQueue = new LinkedBlockingQueue<>(queueCapacity);
        workers = new Thread[size];
        for (int i = 0; i < size; i++) {
            workers[i] = new Worker();
            workers[i].start();
        }
    }

    public void addTask(Task task) throws InterruptedException {
        if (isRunning) {
            taskQueue.put(task);
        }
    }

    public int getQueueSize() {
        return taskQueue.size();
    }

    public int getCompletedTasks() {
        return completedTasks;
    }

    public void shutdown() {
        isRunning = false;
        for (Thread worker : workers) {
            worker.interrupt();
        }
    }

    private class Worker extends Thread {
        @Override
        public void run() {
            while (isRunning && !isInterrupted()) {
                try {
                    Task task = taskQueue.take();
                    task.execute();
                    completedTasks++;
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                }
            }
        }
    }
}