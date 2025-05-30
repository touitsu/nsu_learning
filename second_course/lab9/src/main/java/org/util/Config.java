package org.util;

import java.io.*;
import java.util.Properties;

public class Config {
    public final int bodyStorageSize;
    public final int engineStorageSize;
    public final int accessoryStorageSize;
    public final int carStorageSize;
    public final int bodySuppliersCount;
    public final int engineSuppliersCount;
    public final int accessorySuppliers;
    public final int workers;
    public final int dealers;
    public final boolean logSales;
    public final int threadPoolQueueSize;

    public Config(String filename) throws Exception {
        Properties props = new Properties();
        File configFile = new File(filename);

        if (configFile.exists()) {
            try (FileInputStream in = new FileInputStream(configFile)) {
                props.load(in);
            }
        } else {
            try (InputStream in = getClass().getClassLoader().getResourceAsStream(filename)) {
                if (in == null) throw new FileNotFoundException("Config file not found: " + filename);
                props.load(in);
            }
        }

        this.bodyStorageSize = Integer.parseInt(props.getProperty("StorageBodySize"));
        this.engineStorageSize = Integer.parseInt(props.getProperty("StorageMotorSize"));
        this. accessoryStorageSize = Integer.parseInt(props.getProperty("StorageAccessorySize"));
        this.carStorageSize = Integer.parseInt(props.getProperty("StorageAutoSize"));
        this.bodySuppliersCount = Integer.parseInt(props.getProperty("BodySuppliers", "1"));
        this.engineSuppliersCount = Integer.parseInt(props.getProperty("EngineSuppliers", "1"));
        this.accessorySuppliers = Integer.parseInt(props.getProperty("AccessorySuppliers"));
        this.workers = Integer.parseInt(props.getProperty("Workers"));
        this.dealers = Integer.parseInt(props.getProperty("Dealers"));
        this.logSales = Boolean.parseBoolean(props.getProperty("LogSale"));
        this.threadPoolQueueSize = Integer.parseInt(props.getProperty("ThreadPoolQueueSize", "50"));
    }
}