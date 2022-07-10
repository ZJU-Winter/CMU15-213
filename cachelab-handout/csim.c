/** Author: Winter **/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "cachelab.h"
#include "csim.h"

unsigned long getSet(unsigned long address, int setSize, int blockSize) {
    return (address >> blockSize) & get0x(setSize);
}

unsigned long getTag(unsigned long address, int setSize, int blockSize) {
    return address >> (blockSize + setSize);
}

unsigned long get0x(int offset) {
    unsigned long rst = 1;
    while (offset > 1) {
        offset -= 1;
        rst = (rst << 1) + 1;
    }
    return rst;
}

void unitTest(int setSize, int lineSize, int blockSize, cacheMemory memory) {
    char * address = "1210";
    printf("Set:%ld\n", getSet(strtol(address, NULL, 16), setSize, blockSize));
    printf("Tag:%ld\n", getTag(strtol(address, NULL, 16), setSize, blockSize));

    printf("valid:%d, tag:%ld", memory[0].lines[lineSize - 1].valid, memory[0].lines[0].tag);
}

void load_store(unsigned long tag, unsigned long set, cacheMemory memory) {

}

void modify(unsigned long tag, unsigned long set, cacheMemory memory) {

}

void cacheSimulator(int setSize, int lineSize, int blockSize, char *filePath) {
    FILE *fp = fopen(filePath, "r");
    char *instruction = malloc(20);
    char *address = malloc(17);
    cacheMemory memory = (cacheSet*) malloc((int)pow(2, setSize) * sizeof(cacheSet));
    if (!memory) {
        printf("cacheMemory allocated failed.");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < (int)pow(2, setSize); i += 1) {
        memory[i].lines = (cacheLine*) malloc(lineSize * sizeof(cacheLine));
        if (!memory[i].lines) {
            printf("cacheSet %d allocated failed.", i);
            exit(EXIT_FAILURE);
        }
        memory[i].validCount = 0;
    }
    unsigned long set = 0;
    unsigned long tag = 0;
    unsigned long address_long = 0;
    char accessType = 'A';
    if (!fp) {
        printf("file doesn't exist.\n");
        exit(EXIT_FAILURE);
    }
    while (fgets(instruction, 20, fp)) {
        if (instruction[0] == 'I') {
            continue;
        } else {
            sscanf(instruction," %*[^ ] %[^,]", address);
            accessType = instruction[1];
            address_long = strtol(address, NULL, 16);
            set = getSet(address_long, setSize, blockSize);
            tag = getTag(address_long, setSize, blockSize);
            switch (accessType)
            {
            case 'S':
            case 'L':
                load_store(set, tag, memory);
                break;
            case 'M':
                modify(set, tag, memory);
                break;
            default:
                printf("wrong access type.\n");
                exit(EXIT_FAILURE);
                break;
            }
        }
    }
    free(instruction);
    free(address);
    for (int i = 0; i < (int)pow(2, setSize); i += 1) {
        free(memory[i].lines);
    }
    free(memory);
    fclose(fp);
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
    cacheSimulator(setSize, lineSize, blockSize, filePath);
    printSummary(hits, misses, evictions);
    //unitTest(setSize, lineSize, blockSize);
    return 0;
}