#ifndef CSIM_HEAD
#define CSIM_HEAD
#define MAX(a, b) ((a) > (b) ? (a) : (b))
typedef enum {false, true} boolean;

typedef struct cacheLine{
    boolean valid;
    unsigned int lastTime;
    unsigned long tag;
} cacheLine;

typedef struct cacheSet{
    unsigned long validCount;
    unsigned long maxLastTime;
    cacheLine *lines;
} cacheSet, *cacheMemory;

boolean help = false;
boolean verbose = false;
int hits = 0;
int misses = 0;
int evictions = 0;

void cacheSimulator(int setSize, int lineSize, int blockSize, char *filePath);
unsigned long getSet(unsigned long address, int setSize, int blockSize);
unsigned long getTag(unsigned long address, int setSize, int blockSize);
unsigned long get0x(int offset);
void load_store(char *instruction, unsigned long tag, unsigned long set, int setSize, int lineSize, cacheMemory memory);
void modify(char *instruction, unsigned long tag, unsigned long set, int setSize, int lineSize, cacheMemory memory);
#endif