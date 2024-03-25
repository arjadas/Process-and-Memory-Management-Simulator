

// -f processes.txt -m infinite -q 3.

#define NUM_ARGS 6
#define INFINITE 0
#define FIRST_FIT 1
#define PAGED 2
#define VIRTUAL 3

typedef struct
{
    char *filename;
    int memory;
    int quantum;
} read_t;

read_t *get_inputs(char const *argv[]);
int get_strategy(char *strategy);
