#define MAX_SIZE 256
#define CODE_SIZE 3

typedef struct Airport Airport;
typedef union Union Union;

union Union;
struct Airport;

struct Airport
{
    char Name[MAX_SIZE];
    char Code[CODE_SIZE];
};

union Union {
    Airport airport;

    char Buffer[sizeof(struct Airport)];
};