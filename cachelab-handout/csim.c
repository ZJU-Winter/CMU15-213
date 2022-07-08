/** Author: Winter **/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include "cachelab.h"

typedef enum {true, false} boolean;

typedef struct {
    boolean valid;
    char *tag;
    char *block;
} cacheLine;

typedef struct {
    int validCount;
    cacheLine *lines;
} cacheSet, *cacheMemory;

boolean help = false;
boolean verbose = false;
int hits = 0;
int misses = 0;
int evictions = 0;

void cacheSimulator(int setSize, int lineSize, int blockSize, char *filePath) {

}


int main(int argc, char *argv[])
{
    int opt, setSize, lineSize, blockSize;
    char *filePath = malloc(20);
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch(opt) {
            case 'h':
                help = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 's':
                setSize = atoi(optarg);
                break;
            case 'E':
                lineSize = atoi(optarg);
                break;
            case 'b':
                blockSize = atoi(optarg);
                break;
            case 't':
                strcpy(filePath, optarg);
                break;
            default:
                fprintf(stderr, "Usage: ./csim [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n");
                exit(EXIT_FAILURE);
        }
    }
    cacheSimulation(setSize, lineSize, blockSize, filePath);
    printSummary(hits, misses, evictions);
    return 0;
}