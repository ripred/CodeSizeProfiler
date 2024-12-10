#include <CodeSizeProfiler.h>

static bool DebugOutput = false;

void foo() {
    if (DebugOutput) {
        Serial.println("Running foo...");
    }

    Wrap({
        asm volatile ("" ::: "memory"); // Prevent optimization
        if (DebugOutput) {
            Serial.println("Hello, world!");
        }
        volatile int result = 42;
        result += 42;
    });
}

void bar() {
    if (DebugOutput) {
        Serial.println("Running bar...");
    }

    Wrap({
        volatile int result = 42;
        for (int i = 0; i < 10; ++i) {
            if (DebugOutput) {
                Serial.println(i);
            }
            result += 42;
        }
    });
}

void minimal() {
    if (DebugOutput) {
        Serial.println("Running minimal...");
    }
    Wrap({});
}

void extended_test() {
    if (DebugOutput) {
        Serial.println("Running extended_test...");
    }

    Wrap({
        volatile int result = 42;
        for (int i = 0; i < 100; ++i) {
            if (DebugOutput) {
                Serial.print(i);
            }
            result += random(1, 100000L);
        }
        if (result & 1) {
            result *= 2;
        }
        else {
            result /= 2;
        }

    });
}

void setup() {
    Serial.begin(115200);
    unsigned long start = millis();
    while (!Serial && (millis() - start) < 1000) {
        // Wait for Serial to initialize
    }

    // Call our functions, which will measure their sizes
    calibrate();
    foo();
    bar();
    minimal();
    extended_test();

    // Now that we've collected data, we use the report function from the library.
    // Example: sort by Size in Descending order
    report(Serial, Size, Descending);
}

void loop() {
    // Nothing to do here
}

