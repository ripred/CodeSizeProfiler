#ifndef CODE_SIZE_PROFILER_H
#define CODE_SIZE_PROFILER_H

#include <Arduino.h>

// Metadata structure to store function information
struct FunctionInfo {
    const char* name; // Function name
    long size;        // Function size in bytes (signed)
};

// Global metadata array with static allocation size
#ifndef MAX_FUNCTIONS
#define MAX_FUNCTIONS 10
#endif

extern FunctionInfo function_sizes[MAX_FUNCTIONS];
extern volatile int function_count;

// We will store the calibration size here as well
extern long calibration_size;

// Enums for sorting and ordering
enum SortBy_t { None, Name, Size };
enum OrderBy_t { Ascending, Descending };

// UNUSED macro to suppress unused variable warnings
#define UNUSED(A) do { (void)(A); } while(false)

// Internal macros to implement overloading behavior
#define WRAP_SELECT(_1, _2, NAME, ...) NAME

// Wrap version that takes CODE and a variable (VARNAME)
#define WRAP_WITH_VAR(CODE, VARNAME)                          \
    {                                                         \
        static void* func_start##__COUNTER__ = &&func_start##__COUNTER__; \
        UNUSED(func_start##__COUNTER__);                      \
        func_start##__COUNTER__:                              \
        CODE                                                  \
    label##__COUNTER__:                                       \
        VARNAME = abs((long)((intptr_t)&&label##__COUNTER__ - (intptr_t)func_start##__COUNTER__)); \
        function_sizes[function_count++] = {__FUNCTION__, VARNAME}; \
    }

// Wrap version that only takes CODE and does not require a variable
#define WRAP_NO_VAR(CODE)                                     \
    {                                                         \
        static void* func_start##__COUNTER__ = &&func_start##__COUNTER__; \
        UNUSED(func_start##__COUNTER__);                      \
        func_start##__COUNTER__:                              \
        CODE                                                  \
    label##__COUNTER__:                                       \
        long temp_size##__COUNTER__ = abs((long)((intptr_t)&&label##__COUNTER__ - (intptr_t)func_start##__COUNTER__)); \
        function_sizes[function_count++] = {__FUNCTION__, temp_size##__COUNTER__}; \
    }

// Main Wrap macro that selects between WRAP_NO_VAR and WRAP_WITH_VAR
#define Wrap(...) WRAP_SELECT(__VA_ARGS__, WRAP_WITH_VAR, WRAP_NO_VAR)(__VA_ARGS__)

// Reporting function
void report(Stream &stream, SortBy_t sortby, OrderBy_t orderby = Descending);
void calibrate();

#endif // CODE_SIZE_PROFILER_H

