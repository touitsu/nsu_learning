package org.ui;

import org.storages.*;
import org.suppliers.*;
import org.threadpool.ThreadPool;
import org.util.Config;
import org.util.Logger;

import javax.swing.*;
import java.awt.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class FactoryFrame extends JFrame {
    private final Timer timer = new Timer(100, e -> updateStats());
    private final StatsPanel statsPanel = new StatsPanel();
    private final JSlider bodySlider = new JSlider(0, 2000, 1000);
    private final JSlider engineSlider = new JSlider(0, 2000, 1000);
    private final JSlider accessorySlider = new JSlider(0, 2000, 1000);
    private final JSlider dealerSlider = new JSlider(0, 5000, 2000);

    private final BodyStorage bodyStorage;
    private final EngineStorage engineStorage;
    private final AccessoryStorage accessoryStorage;
    private final CarStorage carStorage;
    private final BodySupplier[] bodySuppliers;
    private final EngineSupplier[] engineSuppliers;
    private final AccessorySupplier[] accessorySuppliers;
    private final Dealer[] dealers;
    private final ThreadPool threadPool;

    public FactoryFrame(Config config,
                        BodyStorage bodyStorage,
                        EngineStorage engineStorage,
                        AccessoryStorage accessoryStorage,
                        CarStorage carStorage,
                        BodySupplier[] bodySuppliers,
                        EngineSupplier[] engineSuppliers,
                        AccessorySupplier[] accessorySuppliers,
                        Dealer[] dealers,
                        ThreadPool threadPool) {

        this.bodyStorage = bodyStorage;
        this.engineStorage = engineStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
        this.bodySuppliers = bodySuppliers;
        this.engineSuppliers = engineSuppliers;
        this.accessorySuppliers = accessorySuppliers;
        this.dealers = dealers;
        this.threadPool = threadPool;

        setupUI();
        setupSliders();
        timer.start();
    }

    private void setupUI() {
        setTitle("Car Factory");
        setSize(600, 400);
        setLayout(new BorderLayout());
        add(this.statsPanel, BorderLayout.CENTER);

        JPanel sliderPanel = new JPanel(new GridLayout(4, 2));
        sliderPanel.add(new JLabel("Body Supplier Delay:"));
        sliderPanel.add(this.bodySlider);
        sliderPanel.add(new JLabel("Engine Supplier Delay:"));
        sliderPanel.add(this.engineSlider);
        sliderPanel.add(new JLabel("Accessory Supplier Delay:"));
        sliderPanel.add(this.accessorySlider);
        sliderPanel.add(new JLabel("Dealer Delay:"));
        sliderPanel.add(this.dealerSlider);

        add(sliderPanel, BorderLayout.SOUTH);
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                shutdown();
            }
        });
    }

    private void setupSliders() {
        this.bodySlider.addChangeListener(e -> {
            for (BodySupplier supplier : this.bodySuppliers) {
                supplier.setDelay(this.bodySlider.getValue());
            }
        });

        this.engineSlider.addChangeListener(e -> {
            for (EngineSupplier supplier : this.engineSuppliers) {
                supplier.setDelay(this.engineSlider.getValue());
            }
        });

        this.accessorySlider.addChangeListener(e -> {
            for (AccessorySupplier supplier : this.accessorySuppliers) {
                supplier.setDelay(this.accessorySlider.getValue());
            }
        });

        this.dealerSlider.addChangeListener(e -> {
            for (Dealer dealer : this.dealers) {
                dealer.setDelay(this.dealerSlider.getValue());
            }
        });
    }

    private void updateStats() {
        this.statsPanel.updateStats(
                this.bodyStorage.size(),
                this.engineStorage.size(),
                this.accessoryStorage.size(),
                this.carStorage.size(),
                Dealer.getCarsSold(),
                this.threadPool.getQueueSize()
        );
    }

    private void shutdown() {
        this.timer.stop();
        this.threadPool.shutdown();
        Logger.close();
        System.exit(0);
    }
}