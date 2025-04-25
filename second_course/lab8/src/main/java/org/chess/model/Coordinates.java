package org.chess.model;

import org.jetbrains.annotations.NotNull;

public record Coordinates(int x, int y) implements Comparable {

    @Override
    public boolean equals(Object obj) {
        if (obj.getClass() != this.getClass()) {
            return false;
        }
        return this.x == ((Coordinates) obj).x() && this.y == ((Coordinates) obj).y();
    }

    @Override
    public int hashCode() {
        return this.x * 10 + this.y;
    }

    @Override
    public String toString() {
        return "(" + this.x + ", " + this.y + ")";
    }

    public String toChessNotation() {
        return (char)(this.x + 'a') + String.valueOf(this.y + 1);
    }

    @Override
    public int compareTo(@NotNull Object o) {
        if (o.getClass() != this.getClass()) {
            return 0;
        }
        else if (this.hashCode() == o.hashCode()) {
            return 0;
        }
        else if (this.hashCode() > o.hashCode()) {
            return 1;
        }
        else {
            return -1;
        }
    }
}
