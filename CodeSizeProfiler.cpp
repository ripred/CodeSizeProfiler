#include <CodeSizeProfiler.h>
#include <string.h>

// Define the global array for function metadata
FunctionInfo function_sizes[MAX_FUNCTIONS] = {};
volatile int function_count = 0;
long calibration_size = 0; // Store the calibration size globally in the library

// function to get the nominal overhead
void calibrate() {
    // Measure a minimal block to get baseline
    Wrap({}, calibration_size);
}


// Helper functions for sorting
static int compare_by_name_asc(const void* a, const void* b) {
    const FunctionInfo* fa = (const FunctionInfo*)a;
    const FunctionInfo* fb = (const FunctionInfo*)b;
    return strcmp(fa->name, fb->name);
}

static int compare_by_name_desc(const void* a, const void* b) {
    const FunctionInfo* fa = (const FunctionInfo*)a;
    const FunctionInfo* fb = (const FunctionInfo*)b;
    return strcmp(fb->name, fa->name);
}

static int compare_by_size_asc(const void* a, const void* b) {
    const FunctionInfo* fa = (const FunctionInfo*)a;
    const FunctionInfo* fb = (const FunctionInfo*)b;
    long sa = fa->size - calibration_size;
    long sb = fb->size - calibration_size;
    if (sa < sb) return -1;
    if (sa > sb) return 1;
    return 0;
}

static int compare_by_size_desc(const void* a, const void* b) {
    const FunctionInfo* fa = (const FunctionInfo*)a;
    const FunctionInfo* fb = (const FunctionInfo*)b;
    long sa = fa->size - calibration_size;
    long sb = fb->size - calibration_size;
    if (sa < sb) return 1;
    if (sa > sb) return -1;
    return 0;
}

void report(Stream &stream, SortBy_t sortby, OrderBy_t orderby) {
    // Make a copy of the function_sizes array so we don't mutate the original
    FunctionInfo funcs[MAX_FUNCTIONS];
    for (int i = 0; i < function_count; ++i) {
        funcs[i] = function_sizes[i];
    }

    // Determine sorting
    if (sortby == Name) {
        if (orderby == Ascending) {
            qsort(funcs, function_count, sizeof(FunctionInfo), compare_by_name_asc);
        } else {
            qsort(funcs, function_count, sizeof(FunctionInfo), compare_by_name_desc);
        }
    } else if (sortby == Size) {
        if (orderby == Ascending) {
            qsort(funcs, function_count, sizeof(FunctionInfo), compare_by_size_asc);
        } else {
            qsort(funcs, function_count, sizeof(FunctionInfo), compare_by_size_desc);
        }
    }

    stream.println(F("-----------------------------------"));
    stream.println(F("  Function Flash Mem Usage Report"));
    stream.print(F("  Calibration Size (bytes): "));
    stream.println(calibration_size);
    stream.print(F("  Total Tracked Functions: "));
    stream.println(function_count);
    stream.println(F("-----------------------------------"));

    char txt[128] = {0};
    stream.println(F("Name                 Size\n"));
    for (int i = 0; i < function_count; ++i) {
        snprintf(txt, sizeof(txt), "%-18s%6ld",
            funcs[i].name,
            funcs[i].size - calibration_size
        );
        stream.println(txt);
    }
    
    stream.println(F("-----------------------------------"));
    stream.println(F("Report Finished."));
}

