package org.ui;

import javax.swing.*;
import java.awt.*;

public class StatsPanel extends JPanel {
    private final JLabel bodyCount = new JLabel("0");
    private final JLabel engineCount = new JLabel("0");
    private final JLabel accessoryCount = new JLabel("0");
    private final JLabel carCount = new JLabel("0");
    private final JLabel carsSold = new JLabel("0");
    private final JLabel tasksCount = new JLabel("0");

    public StatsPanel() {
        setLayout(new GridLayout(6, 2));
        add(new JLabel("Body Storage:"));
        add(bodyCount);
        add(new JLabel("Engine Storage:"));
        add(engineCount);
        add(new JLabel("Accessory Storage:"));
        add(accessoryCount);
        add(new JLabel("Car Storage:"));
        add(carCount);
        add(new JLabel("Cars Sold:"));
        add(carsSold);
        add(new JLabel("Tasks in Queue:"));
        add(tasksCount);
    }

    public void updateStats(int bodies, int engines, int accessories,
                            int cars, int sold, int tasks) {
        bodyCount.setText(String.valueOf(bodies));
        engineCount.setText(String.valueOf(engines));
        accessoryCount.setText(String.valueOf(accessories));
        carCount.setText(String.valueOf(cars));
        carsSold.setText(String.valueOf(sold));
        tasksCount.setText(String.valueOf(tasks));
    }
}