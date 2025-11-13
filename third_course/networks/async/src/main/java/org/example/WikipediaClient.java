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

public class WikipediaClient {
    private static final String BASE_URL = "https://ru.wikipedia.org/w/api.php";

    private final HttpClient client = HttpClient.newHttpClient();

    public CompletableFuture<List<Place>> searchPlacesNear(double lat, double lon) {
        String coordParam = URLEncoder.encode(lat + "|" + lon, StandardCharsets.UTF_8);
        String url = BASE_URL +
                "?action=query" +
                "&list=geosearch" +
                "&gscoord=" + coordParam +
                "&gsradius=5000" +
                "&gslimit=5" +
                "&format=json" +
                "&gsprimary=primary";

        HttpRequest req = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .timeout(Duration.ofSeconds(10))
                .header("User-Agent", "Async-api/1.0 (+https://github.com/touitsu/nsu_learning; contact: sashak1705@gmail.com)")
                .build();

        return this.client.sendAsync(req, HttpResponse.BodyHandlers.ofString())
                .thenApply(HttpResponse::body)
                .thenApply(this::parseGeosearch)
                .exceptionally(t -> {
                    System.err.println("Wikipedia api threw " + t.getMessage());
                    return Collections.emptyList();
                });
    }

    public CompletableFuture<String> fetchDescription(String title) {
        String encodedTitle = URLEncoder.encode(title, StandardCharsets.UTF_8);
        String url = BASE_URL +
                "?action=query" +
                "&prop=extracts" +
                "&exintro=true" +
                "&explaintext=true" +
                "&exsentences=2" +
                "&titles=" + encodedTitle +
                "&format=json" +
                "&formatversion=2";

        HttpRequest req = HttpRequest.newBuilder()
                .uri(URI.create(url))
                .timeout(Duration.ofSeconds(10))
                .header("User-Agent", "Async-api/1.0 (+https://github.com/touitsu/nsu_learning; contact: sashak1705@gmail.com)")
                .build();

        return this.client.sendAsync(req, HttpResponse.BodyHandlers.ofString())
                .thenApply(HttpResponse::body)
                .thenApply(this::parseExtract)
                .exceptionally(t -> {
                    System.err.println("Failed to get description for '" + title + "': " + t.getMessage());
                    return "";
                });
    }

    private List<Place> parseGeosearch(String json) {
        try {
            JsonObject root = JsonParser.parseString(json).getAsJsonObject();
            JsonObject query = root.getAsJsonObject("query");
            if (query == null) return Collections.emptyList();

            JsonArray geosearch = query.getAsJsonArray("geosearch");
            if (geosearch == null) return Collections.emptyList();

            List<Place> places = new ArrayList<>();
            for (var el : geosearch) {
                JsonObject item = el.getAsJsonObject();
                Place p = new Place();
                p.xid = item.get("pageid").getAsString();
                p.name = item.get("title").getAsString();
                double dist = item.get("dist").getAsDouble();
                p.dist = dist;
                places.add(p);
            }
            return places;
        }
        catch (Exception e) {
            return Collections.emptyList();
        }
    }

    private String parseExtract(String json) {
        try {
            JsonObject root = JsonParser.parseString(json).getAsJsonObject();
            JsonArray pages = root.getAsJsonObject("query").getAsJsonArray("pages");
            if (pages.size() == 0) return "";

            JsonObject page = pages.get(0).getAsJsonObject();

            if (page.has("extract")) {
                return page.get("extract").getAsString().trim();
            }
            else {
                return "— No description —";
            }
        }
        catch (Exception e) {
            System.err.println("Failed to parse description for: " + e.getMessage());
            return "";
        }
    }
}
