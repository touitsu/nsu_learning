package org.chess.model;

import org.chess.model.exceptions.RepetitionException;
import org.jetbrains.annotations.NotNull;

import java.util.ArrayList;

public final class MoveHistory {
    private final ArrayList<String> list;

    public MoveHistory() {
        this.list = new ArrayList<String>();
    }

    public void add(@NotNull String move) throws RepetitionException {
        if (this.list.size() == 5) {

            if (this.list.get(1).equals(this.list.get(3).substring(3) + " " + this.list.get(3).substring(0, 2)) &&
                    this.list.get(0).equals(this.list.get(4)) && this.list.get(2).equals(this.list.get(0).substring(3) + " " + this.list.get(0).substring(0, 2))) {
                throw new RepetitionException("Three moves repetition.\n");
            }
            else {
                this.list.remove(0);
                this.list.add(move);
            }
        }
        else {
            this.list.add(move);
        }
    }
}
