package org.example;

import java.util.List;

public record LocationReport(
        GeocodingResult location,
        String weatherSummary,
        List<PlaceOfInterest> placesOfInterest
) {}