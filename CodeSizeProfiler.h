#ifndef CODE_SIZE_PROFILER_H
#define CODE_SIZE_PROFILER_H

#include <Arduino.h>

// Metadata structure to store function information
struct FunctionInfo {
    const char* name;        // Function name
    long size;               // Function size in bytes (signed)
};

// Global metadata array with static allocation size
#ifndef MAX_FUNCTIONS
#define MAX_FUNCTIONS 10
#endif

extern FunctionInfo function_sizes[MAX_FUNCTIONS];
extern volatile int function_count;

// UNUSED macro to suppress unused variable warnings
#define UNUSED(A) do { (void)(A); } while(false)

// Debug output macro (enable or disable here)
#define DEBUG_OUTPUT 1
#define DebugPrint(msg, value) \
    do { if (debugMode) { Serial.print(msg); Serial.println(value, HEX); } } while (false)

// Wrap macro for measuring flash memory usage
#define Wrap(CODE, VARNAME)                              \
    {                                                    \
        static void* func_start##__COUNTER__ = &&func_start##__COUNTER__; \
        UNUSED(func_start##__COUNTER__);                 \
        func_start##__COUNTER__:                         \
        __asm__ volatile("nop\nnop\nnop\nnop\nnop");      \
        CODE                                             \
    label##__COUNTER__:                                  \
        __asm__ volatile("nop\nnop\nnop\nnop\nnop");      \
        VARNAME = abs((long)((intptr_t)&&label##__COUNTER__ - (intptr_t)func_start##__COUNTER__) - 10);function_sizes[function_count++] = {__FUNCTION__, VARNAME}; \
        DebugPrint("Debug: func_start = ", (intptr_t)func_start##__COUNTER__); \
        DebugPrint("Debug: label = ", (intptr_t)&&label##__COUNTER__); \
        DebugPrint("Debug: size (bytes) = ", VARNAME);   \
    }

#endif // CODE_SIZE_PROFILER_H

