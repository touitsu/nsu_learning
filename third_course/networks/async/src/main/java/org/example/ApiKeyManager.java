package org.example;

import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

public class ApiKeyManager {
    private final Map<String, String> keys;

    public ApiKeyManager(String filePath) {
        this.keys = loadKeys(filePath);
    }

    private Map<String, String> loadKeys(String filePath) {
        Map<String, String> map = new HashMap<>();
        try (BufferedReader reader = Files.newBufferedReader(Path.of(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                int i = line.indexOf(':');
                if (i > 0) {
                    String name = line.substring(0, i).trim();
                    String key = line.substring(i + 1).trim();
                    if (!name.isEmpty() && !key.isEmpty()) {
                        map.putIfAbsent(name, key);
                    }
                }
            }
        }
        catch (IOException e) {
            throw new RuntimeException("Failed to load keys from " + filePath, e);
        }
        return Collections.unmodifiableMap(map);
    }

    public String getKey(String name) {
        return this.keys.get(name);
    }
}