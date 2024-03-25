#include <process.c>

#define FINISHED 0
#define READY 1
#define RUNNING 2

typedef struct {
    char *name;
    int arrival;
    int time_required; // check data type
    int memory_KB;
    int status = -1;
} process_t;