#include "sort.h"
#include <sys/stat.h>
#include <string.h>

// Compare function for qsort by file name ascending
int compare_name_asc(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

// Compare function for qsort by file name descending
int compare_name_desc(const void *a, const void *b)
{
    return -strcmp(*(const char **)a, *(const char **)b);
}

// Compare function for qsort by modification time ascending
int compare_time_asc(const void *a, const void *b)
{
    struct stat stat_a, stat_b;
    if (lstat(*(const char **)a, &stat_a) == -1 || lstat(*(const char **)b, &stat_b) == -1)
    {
        return 0; // Fallback to not changing order
    }
    return stat_a.st_mtime - stat_b.st_mtime;
}

// Compare function for qsort by modification time descending
int compare_time_desc(const void *a, const void *b)
{
    return -compare_time_asc(a, b);
}