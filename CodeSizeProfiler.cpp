#include <CodeSizeProfiler.h>

// Define the global array for function metadata
FunctionInfo function_sizes[MAX_FUNCTIONS] = {};
volatile int function_count = 0;

