import org.StackCalculator.*;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.util.ArrayList;
import java.util.Stack;
import java.util.TreeMap;

import static org.junit.jupiter.api.Assertions.*;

public final class tests {

    private Factory factory;
    private Context context;
    private ArrayList<String> args;

    @BeforeEach
    public void reset() {
        assertDoesNotThrow(() -> this.factory = new Factory());
        this.context = new Context(new Stack<Double>(), new TreeMap<String, Double>());
        this.args = new ArrayList<String>();
    }

    @Test
    public void factoryTest() {
        String[] commands = {"+", "-", "*", "/", "DEFINE", "PRINT", "POP", "PUSH"};

        assertThrowsExactly(FactoryException.class, () -> this.factory.createOperation("ab", 0));

        for (String command : commands) {
            assertDoesNotThrow(() -> this.factory.createOperation(command, 0));
        }

    }

    @Test
    public void pushTest() {
        this.args.add("2");

        assertDoesNotThrow(() -> this.factory.createOperation("PUSH", 0).complete(this.context, this.args));
        assertFalse(this.context.stack().isEmpty());
        assertEquals(2, this.context.stack().get(0));

        this.args.remove(0);
        this.args.add("a");

        assertThrows(OperationException.class, () -> this.factory.createOperation("PUSH", 0).complete(this.context, this.args));

        this.context.map().put("a", 2.0);

        assertDoesNotThrow(() -> this.factory.createOperation("PUSH", 0).complete(this.context, this.args));
    }

    @Test
    public void defineTest() {
        this.args.add("a");

        assertThrows(OperationException.class, () -> this.factory.createOperation("DEFINE", 0).complete(this.context, this.args));

        this.args.add("2");

        assertDoesNotThrow(() -> this.factory.createOperation("DEFINE", 0).complete(this.context, this.args));


        assertTrue(this.context.map().containsKey("a"));
        assertEquals(2.0, this.context.map().get("a"));

        this.args.remove(0);
        this.args.remove(0);
        this.args.add("2.0");
        this.args.add("2.0");

        assertThrows(OperationException.class, () -> this.factory.createOperation("DEFINE", 0).complete(this.context, this.args));
    }

    @Test
    public void printTest() {
        assertThrows(StackException.class, () -> this.factory.createOperation("PRINT", 0).complete(this.context, this.args));

        this.context.stack().push(2.0);

        assertDoesNotThrow(() -> this.factory.createOperation("PRINT", 0).complete(this.context, this.args));
        assertFalse(this.context.stack().isEmpty());
    }


    @Test
    public void popTest() {
        this.context.stack().push(2.0);

        assertDoesNotThrow(() -> this.factory.createOperation("POP", 0).complete(this.context, this.args));
        assertTrue(this.context.stack().isEmpty());
        assertThrows(StackException.class, () -> this.factory.createOperation("POP", 0).complete(this.context, this.args));
    }

    @Test
    public void additionTest() {
        assertThrows(StackException.class, () -> this.factory.createOperation("-", 0).complete(this.context, this.args));

        this.context.stack().push(2.0);

        assertThrows(OperationException.class, () -> this.factory.createOperation("+", 0).complete(this.context, this.args));

        this.context.stack().push(2.0);

        assertDoesNotThrow(() -> this.factory.createOperation("+", 0).complete(this.context, this.args));
        assertFalse(this.context.stack().isEmpty());
        assertEquals(4.0, this.context.stack().get(0));
    }

    @Test
    public void subtractionTest() {
        assertThrows(StackException.class, () -> this.factory.createOperation("-", 0).complete(this.context, this.args));

        this.context.stack().push(2.0);

        assertThrows(StackException.class, () -> this.factory.createOperation("-", 0).complete(this.context, this.args));

        this.context.stack().push(2.0);

        assertDoesNotThrow(() -> this.factory.createOperation("-", 0).complete(this.context, this.args));
        assertFalse(this.context.stack().isEmpty());
        assertEquals(0.0, this.context.stack().get(0));
    }

    @Test
    public void multiplicationTest() {
        assertThrows(OperationException.class, () -> this.factory.createOperation("*", 0).complete(this.context, this.args));

        this.context.stack().push(10.0);

        assertThrows(OperationException.class, () -> this.factory.createOperation("*", 0).complete(this.context, this.args));

        this.context.stack().push(10.0);

        assertDoesNotThrow(() -> this.factory.createOperation("*", 0).complete(this.context, this.args));
        assertFalse(this.context.stack().isEmpty());
        assertEquals(100.0, this.context.stack().get(0));
    }

    @Test
    public void divisionTest() {
        assertThrows(OperationException.class, () -> this.factory.createOperation("/", 0).complete(this.context, this.args));

        this.context.stack().push(2.0);

        assertThrows(OperationException.class, () -> this.factory.createOperation("/", 0).complete(this.context, this.args));

        this.context.stack().push(10.0);

        assertDoesNotThrow(() -> this.factory.createOperation("/", 0).complete(this.context, this.args));
        assertFalse(this.context.stack().isEmpty());
        assertEquals(5.0, this.context.stack().get(0));

        this.context.stack().pop();
        this.context.stack().push(0.0);
        this.context.stack().push(1.0);

        assertThrows(OperationException.class, () -> this.factory.createOperation("/", 0).complete(this.context, this.args));
    }
}
