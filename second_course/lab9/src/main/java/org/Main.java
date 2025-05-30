package org;

import org.controller.ProductionController;
import org.factory.*;
import org.storages.*;
import org.suppliers.*;
import org.threadpool.ThreadPool;
import org.ui.FactoryFrame;
import org.util.Config;
import org.util.Logger;

import javax.swing.*;

public class Main {
    public static void main(String[] args) throws Exception {
        Config config = new Config("factory.config");
        if (config.logSales) {
            Logger.init("sales.log");
        }

        // Инициализация складов
        BodyStorage bodyStorage = new BodyStorage(config.bodyStorageSize);
        EngineStorage engineStorage = new EngineStorage(config.engineStorageSize);
        AccessoryStorage accessoryStorage = new AccessoryStorage(config.accessoryStorageSize);
        CarStorage carStorage = new CarStorage(config.carStorageSize);

        // Создание поставщиков для всех типов деталей
        BodySupplier[] bodySuppliers = createSuppliers(
                BodySupplier.class, bodyStorage, config.bodySuppliersCount, 1000);

        EngineSupplier[] engineSuppliers = createSuppliers(
                EngineSupplier.class, engineStorage, config.engineSuppliersCount, 1000);

        AccessorySupplier[] accessorySuppliers = createSuppliers(
                AccessorySupplier.class, accessoryStorage, config.accessorySuppliers, 500);

        // Создание дилеров
        Dealer[] dealers = new Dealer[config.dealers];
        for (int i = 0; i < config.dealers; i++) {
            dealers[i] = new Dealer(carStorage, i + 1, 3000);
        }

        // Пул потоков с ограниченной очередью
        ThreadPool threadPool = new ThreadPool(config.workers, config.threadPoolQueueSize);

        // Контроллер производства
        ProductionController controller = new ProductionController(
                carStorage, threadPool, bodyStorage, engineStorage, accessoryStorage
        );
        Thread controllerThread = new Thread(controller);
        controllerThread.setDaemon(true);
        controllerThread.start();

        // Запуск потоков
        startDaemonThreads(bodySuppliers);
        startDaemonThreads(engineSuppliers);
        startDaemonThreads(accessorySuppliers);
        startDaemonThreads(dealers);

        // Графический интерфейс
        SwingUtilities.invokeLater(() ->
                new FactoryFrame(config, bodyStorage, engineStorage, accessoryStorage, carStorage,
                        bodySuppliers, engineSuppliers, accessorySuppliers, dealers, threadPool)
                        .setVisible(true)
        );
    }

    @SuppressWarnings("unchecked")
    private static <T extends Supplier<?>> T[] createSuppliers(
            Class<T> supplierClass, Storage storage, int count, int baseDelay)
            throws Exception {

        T[] suppliers = (T[]) java.lang.reflect.Array.newInstance(supplierClass, count);
        int individualDelay = baseDelay * count; // Балансировка скорости

        for (int i = 0; i < count; i++) {
            if (supplierClass == BodySupplier.class) {
                suppliers[i] = (T) new BodySupplier((Storage<Body>) storage, individualDelay);
            } else if (supplierClass == EngineSupplier.class) {
                suppliers[i] = (T) new EngineSupplier((Storage<Engine>) storage, individualDelay);
            } else if (supplierClass == AccessorySupplier.class) {
                suppliers[i] = (T) new AccessorySupplier((Storage<Accessory>) storage, individualDelay);
            }
        }
        return suppliers;
    }

    private static void startDaemonThreads(Runnable[] runnables) {
        for (Runnable r : runnables) {
            Thread thread = new Thread(r);
            thread.setDaemon(true);
            thread.start();
        }
    }
}