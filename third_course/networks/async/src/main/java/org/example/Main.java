package org.example;


import java.util.List;
import java.util.Scanner;
import java.util.concurrent.CompletionException;

public class Main {
    public static void main(String[] args) {
        ApiKeyManager keys = new ApiKeyManager("keys");
        String gh = keys.getKey("graphhopper");
        String ow = keys.getKey("openweather");

        if (gh == null || ow == null) {
            System.err.println("API keys missing.");
            return;
        }

        LocationInfo service = new LocationInfo(gh, ow);
        Scanner in = new Scanner(System.in);

        System.out.print("Location name: ");
        String query = in.nextLine().trim();
        if (query.isEmpty()) return;

        List<GeocodingResult> locations;
        try {
            locations = service.getGhClient().searchLocations(query).join();
        }
        catch (Exception e) {
            System.err.println("Geocoding failed: " + e.getMessage());
            return;
        }

        if (locations.isEmpty()) {
            System.out.println("Nothing found.");
            return;
        }

        for (int i = 0; i < locations.size(); i++) {
            GeocodingResult loc = locations.get(i);
            System.out.printf("%d. %s (%.4f, %.4f)\n", i + 1, loc.address(), loc.lon(), loc.lat());
        }

        System.out.print("\nNumber: ");
        int choice = readInt(new Scanner(System.in), 1, locations.size());
        if (choice == -1) return;

        try {
            LocationReport report = service.fetchReport(query, choice).join();

            System.out.println("\nSelected: " + report.location().address());
            System.out.println(report.weatherSummary());

            System.out.println("\nPlaces of interest:");
            if (report.placesOfInterest().isEmpty()) {
                System.out.println("— None found —");
            } else {
                report.placesOfInterest().forEach(p ->
                        System.out.printf("- %s\n  %s\n\n", p.name(), p.description())
                );
            }

        }
        catch (CompletionException e) {
            System.err.println("Report generation failed: " + e.getCause().getMessage());
        }
    }

    private static int readInt(Scanner in, int min, int max) {
        try {
            int n = Integer.parseInt(in.nextLine().trim());
            return (n >= min && n <= max) ? n : -1;
        }
        catch (NumberFormatException e) {
            return -1;
        }
    }
}