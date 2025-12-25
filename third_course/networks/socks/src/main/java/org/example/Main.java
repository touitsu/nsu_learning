package org.example;

import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        if (args.length != 1) {
            System.err.println("Usage: java Main <port>");
            System.err.println("Example: java Main 1080");
            System.exit(1);
        }

        try {
            int port = Integer.parseInt(args[0]);

            if (port < 1 || port > 65535) {
                System.err.println("Port must be between 1 and 65535");
                System.exit(1);
            }

            Socks5Proxy proxy = new Socks5Proxy(port);
            System.out.println("Starting SOCKS5 proxy server on port " + port + "...");
            proxy.start();

        } catch (NumberFormatException e) {
            System.err.println("Invalid port number: " + args[0]);
            System.exit(1);
        } catch (IOException e) {
            System.err.println("Failed to start proxy server: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }
}