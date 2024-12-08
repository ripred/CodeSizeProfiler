#include <CodeSizeProfiler.h>

// Define global variables for function sizes
long foo_size = 0;
long bar_size = 0;
long minimal_size = 0;
long extended_test_size = 0;
long calibration_size = 0;

void calibrate() {
    Serial.println("Running calibration...");
    Wrap({}, calibration_size);
}

void foo() {
    Serial.println("Running foo...");
    Wrap({
        asm volatile ("" ::: "memory"); // Prevent optimization
        Serial.println("Hello, world!");
    }, foo_size);
}

void bar() {
    Serial.println("Running bar...");
    Wrap({
        for (int i = 0; i < 10; ++i) {
            Serial.println(i);
        }
    }, bar_size);
}

void minimal() {
    Serial.println("Running minimal...");
    Wrap({}, minimal_size); // Minimal code block
}

void extended_test() {
    Serial.println("Running extended_test...");
    Wrap({
        for (int i = 0; i < 100; ++i) {
            Serial.print(i);
        }
    }, extended_test_size);
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {
        // Wait for Serial to initialize
    }

    calibrate();
    foo();
    bar();
    minimal();
    extended_test();

    Serial.println("Function Memory Usage Report:");
    Serial.print("Calibration Size (bytes): ");
    Serial.println(calibration_size);

    for (int i = 0; i < function_count; ++i) {
        Serial.print("Function: ");
        Serial.print(function_sizes[i].name);
        Serial.print(", Adjusted Size (bytes): ");
        Serial.println(function_sizes[i].size - calibration_size); // Adjusted size
    }
}

void loop() {
    // Nothing to do here
}

