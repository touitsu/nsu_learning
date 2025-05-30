package org.util;

import org.factory.Car;
import java.io.FileWriter;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;

public class Logger {
    private static FileWriter writer;
    private static final DateTimeFormatter formatter =
            DateTimeFormatter.ofPattern("HH:mm:ss");

    public static void init(String filename) throws Exception {
        writer = new FileWriter(filename, true);
    }

    public static void logSale(int dealerId, Car car) {
        if (writer == null) return;

        String time = LocalTime.now().format(formatter);
        String log = String.format("%s: Dealer %d: Auto %d (Body: %d, Motor: %d, Accessory: %d)%n",
                time, dealerId, car.getId(),
                car.getBody().getId(), car.getEngine().getId(), car.getAccessory().getId());

        try {
            writer.write(log);
            writer.flush();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void close() {
        try {
            if (writer != null) writer.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}