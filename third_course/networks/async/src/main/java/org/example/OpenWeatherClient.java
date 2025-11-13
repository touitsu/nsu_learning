package org.example;

import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.time.Duration;
import java.util.concurrent.CompletableFuture;

public class OpenWeatherClient {
    private final String apiKey;
    private final HttpClient client = HttpClient.newHttpClient();

    public OpenWeatherClient(String apiKey) {
        this.apiKey = apiKey;
    }

    public CompletableFuture<String> getWeather(double lat, double lon) {
        String url = "https://api.openweathermap.org/data/2.5/weather?lat=%f&lon=%f&appid=%s&units=metric&lang=ru"
                .formatted(lat, lon, this.apiKey);

        HttpRequest req = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .timeout(Duration.ofSeconds(10))
                .build();

        return this.client.sendAsync(req, HttpResponse.BodyHandlers.ofString())
                .thenApply(HttpResponse::body)
                .thenApply(this::formatWeather)
                .exceptionally(t -> ("Openweather threw " +  t.getMessage()));
    }

    private String formatWeather(String json) {
        try {
            JsonObject root = JsonParser.parseString(json).getAsJsonObject();
            if (root.get("cod").getAsInt() != 200) return "Openweather returned http code != 200";
            double temp = root.getAsJsonObject("main").get("temp").getAsDouble();
            String desc = root.getAsJsonArray("weather").get(0)
                    .getAsJsonObject().get("description").getAsString();
            return "Weather: %s, %.1f°C".formatted(desc, temp);
        }
        catch (Exception e) {
            return "Failed to fetch weather";
        }
    }
}
