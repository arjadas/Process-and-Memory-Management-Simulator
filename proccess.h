#include <proccess.c>

typedef struct {
    char *name;
    int arrival;
    int time_required; // check data type
    int memory_KB;
    int RUNNING;
    int READY;
    int FINISHED;
} proccess_t;