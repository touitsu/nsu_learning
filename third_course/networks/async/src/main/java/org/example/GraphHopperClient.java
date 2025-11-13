package org.example;

import com.google.gson.JsonArray;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import java.net.URI;
import java.net.URLEncoder;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.nio.charset.StandardCharsets;
import java.time.Duration;
import java.util.*;
import java.util.concurrent.CompletableFuture;

public class GraphHopperClient {
    private final String apiKey;
    private final HttpClient client = HttpClient.newHttpClient();

    public GraphHopperClient(String apiKey) {
        this.apiKey = apiKey;
    }

    public CompletableFuture<List<GeocodingResult>> searchLocations(String query) {
        String encoded = URLEncoder.encode(query, StandardCharsets.UTF_8);
        String url = "https://graphhopper.com/api/1/geocode?q=%s&key=%s&limit=5&locale=ru"
                .formatted(encoded, this.apiKey);

        HttpRequest req = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .timeout(Duration.ofSeconds(10))
                .build();

        return this.client.sendAsync(req, HttpResponse.BodyHandlers.ofString())
                .thenApply(HttpResponse::body)
                .thenApply(this::parse)
                .exceptionally(t -> {
                    System.err.println("GraphHopper client threw : " + t.getMessage());
                    return Collections.emptyList();
                });
    }

    private List<GeocodingResult> parse(String json) {
        try {
            JsonObject root = JsonParser.parseString(json).getAsJsonObject();
            JsonArray hits = root.getAsJsonArray("hits");
            if (hits == null) return List.of();

            List<GeocodingResult> results = new ArrayList<>();
            for (var el : hits) {
                JsonObject h = el.getAsJsonObject();
                String name = h.get("name").getAsString();
                JsonObject p = h.getAsJsonObject("point");
                double lat = p.get("lat").getAsDouble();
                double lon = p.get("lng").getAsDouble();
                results.add(new GeocodingResult(name, lat, lon));
            }
            return results;
        }
        catch (Exception e) {
            return List.of();
        }
    }
}
