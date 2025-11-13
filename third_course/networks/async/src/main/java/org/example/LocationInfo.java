package org.example;

import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.stream.Collectors;

public class LocationInfo {

    private final GraphHopperClient ghClient;
    private final OpenWeatherClient owClient;
    private final WikipediaClient wikiClient;

    public LocationInfo(String graphhopperKey, String openweatherKey) {
        this.ghClient = new GraphHopperClient(graphhopperKey);
        this.owClient = new OpenWeatherClient(openweatherKey);
        this.wikiClient = new WikipediaClient();
    }

    public CompletableFuture<LocationReport> fetchReport(String locationName, int selectedIndex) {
        return ghClient.searchLocations(locationName)
                .thenCompose(locations -> {
                    if (locations.isEmpty()) {
                        return CompletableFuture.failedFuture(
                                new IllegalArgumentException("No locations found for: " + locationName));
                    }
                    if (selectedIndex < 1 || selectedIndex > locations.size()) {
                        return CompletableFuture.failedFuture(
                                new IllegalArgumentException("Invalid selection index: " + selectedIndex));
                    }

                    GeocodingResult selected = locations.get(selectedIndex - 1);
                    double lat = selected.lat();
                    double lon = selected.lon();

                    CompletableFuture<String> weatherFuture =
                            owClient.getWeather(lat, lon)
                                    .exceptionally(t -> "Weather unavailable: " + t.getMessage());

                    CompletableFuture<List<?>> placesFuture =
                            wikiClient.searchPlacesNear(lat, lon)
                                    .thenCompose(places -> {
                                        if (places.isEmpty()) {
                                            return CompletableFuture.completedFuture(List.of());
                                        }

                                        List<CompletableFuture<PlaceOfInterest>> descFutures = places.stream()
                                                .map(p -> wikiClient.fetchDescription(p.name())
                                                        .thenApply(desc -> new PlaceOfInterest(
                                                                p.name(),
                                                                desc == null || desc.isEmpty() ? "— No description —" : desc
                                                        )))
                                                .toList();

                                        return CompletableFuture.allOf(descFutures.toArray(CompletableFuture[]::new))
                                                .thenApply(__ ->
                                                        descFutures.stream().map(CompletableFuture::join).toList()
                                                );
                                    })
                                    .exceptionally(t -> {
                                        System.err.println("Wikipedia fetch failed: " + t.getMessage());
                                        return List.of();
                                    });

                    return weatherFuture
                            .thenCombine(placesFuture, (weather, places) ->
                                    new LocationReport(selected, weather, (List<PlaceOfInterest>) places)
                            );
                });
    }

    public GraphHopperClient getGhClient() {
        return this.ghClient;
    }
}