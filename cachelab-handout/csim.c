/** Author: Winter **/
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <limits.h>
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

void test(int setSize, int lineSize, int blockSize, cacheMemory memory) {
    /*
    char * address = "1210";
    printf("Set:%ld\n", getSet(strtol(address, NULL, 16), setSize, blockSize));
    printf("Tag:%ld\n", getTag(strtol(address, NULL, 16), setSize, blockSize));
    */
    for (int i = 0; i < (int)pow(2,setSize); i += 1) {
        printf("------Set %2d------\n", i);
        for (int j = 0; j < lineSize; j += 1) {
            printf("valid:%d, tag:%ld, lastTime:%d\n", memory[i].lines[j].valid, memory[i].lines[j].tag, memory[i].lines[j].lastTime);
        }
    }
    printf("\n\n");
}

void load_store(char *instruction, unsigned long tag, unsigned long set, int setSize, int lineSize, cacheMemory memory) {
    //cacheLine* lines = memory[set].lines;
    //printf("instruction %s step in load_store, tag is %ld set is %ld\n", instruction, tag, set);
    int validCount = memory[set].validCount;
    unsigned int maxLastTime = 0;
    unsigned int minLastTime = UINT_MAX;
    unsigned int minIndex = 0;
    for (int i = 0; i < validCount; i += 1) {
        maxLastTime = MAX(maxLastTime, memory[set].lines[i].lastTime);
    }
    for (int i = 0; i < validCount; i += 1) {
        if (memory[set].lines[i].valid == true && memory[set].lines[i].tag == tag) {
            // hits
            hits += 1;
            memory[set].lines[i].lastTime = maxLastTime + 1;
            if (verbose) {
                printf("%s hit\n", instruction);
            }
            return;
        }
    }
    // doesn't match any tag
    misses += 1;
    if (validCount != lineSize) {
        memory[set].lines[validCount].tag = tag;
        memory[set].lines[validCount].valid = true;
        memory[set].lines[validCount].lastTime = maxLastTime + 1;
        memory[set].validCount += 1;
        if (verbose) {
            printf("%s miss\n", instruction);
        }
        return;
    }
    // needs to replace something
    evictions += 1;
    for (int i = 0; i < lineSize; i += 1) {
        if (memory[set].lines[i].lastTime < minLastTime) {
            minLastTime = memory[set].lines[i].lastTime;
            minIndex = i;
        }
    }
    memory[set].lines[minIndex].tag = tag;
    memory[set].lines[minIndex].lastTime = maxLastTime + 1;
    if (verbose) {
        printf("%s miss eviction\n",instruction);
    }
}

void modify(char *instruction, unsigned long tag, unsigned long set, int setSize, int lineSize, cacheMemory memory) {
    int validCount = memory[set].validCount;
    unsigned int maxLastTime = 0;
    unsigned int minLastTime = UINT_MAX;
    unsigned int minIndex = 0;
    for (int i = 0; i < validCount; i += 1) {
        maxLastTime = MAX(maxLastTime, memory[set].lines[i].lastTime);
        if (memory[set].lines[i].valid == true && memory[set].lines[i].tag == tag) {
            // hits
            hits += 2;
            memory[set].lines[i].lastTime = maxLastTime + 2;
            if (verbose) {
                printf("%s hit hit\n", instruction);
            }
            return;
        }
    }
    // doesn't match any tag
    misses += 1;
    hits += 1;
    if (validCount != lineSize) {
        memory[set].lines[validCount].tag = tag;
        memory[set].lines[validCount].valid = true;
        memory[set].lines[validCount].lastTime = maxLastTime + 2;
        memory[set].validCount += 1;
        if (verbose) {
            printf("%s miss hit\n", instruction);
        }
        return;
    }
    // needs to replace something
    evictions += 1;
    for (int i = 0; i < lineSize; i += 1) {
        if (memory[set].lines[i].lastTime < minLastTime) {
            minLastTime = memory[set].lines[i].lastTime;
            minIndex = i;
        }
    }
    memory[set].lines[minIndex].tag = tag;
    memory[set].lines[minIndex].lastTime = maxLastTime + 2;
    if (verbose) {
        printf("%s miss eviction hit\n",instruction);
    }
}

void cacheInit(int setSize, int lineSize, cacheMemory memory) {
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
}

void cacheSimulator(int setSize, int lineSize, int blockSize, char *filePath) {
    cacheMemory memory = (cacheSet*) malloc((int)pow(2, setSize) * sizeof(cacheSet));
    char *instruction = malloc(20);
    char *address = malloc(17);
    unsigned long set = 0;
    unsigned long tag = 0;
    unsigned long address_long = 0;
    char accessType = 'A';
    FILE *fp = fopen(filePath, "r");
    if (!fp) {
        printf("file doesn't exist.\n");
        exit(EXIT_FAILURE);
    }
    cacheInit(setSize, lineSize, memory);
    //test(setSize, lineSize, blockSize, memory);
    while (fgets(instruction, 20, fp)) {
        //remove the newline byte
        instruction[strcspn(instruction, "\n")] = 0;
        if (instruction[0] == 'I') {
            continue;
        } else {
            sscanf(instruction," %*[^ ] %[^,]", address);
            accessType = instruction[1];
            //printf("instruction:%s\naddress:%s accessType:%c\n",instruction, address, accessType);
            address_long = strtol(address, NULL, 16);
            set = getSet(address_long, setSize, blockSize);
            tag = getTag(address_long, setSize, blockSize);
            switch (accessType)
            {
            case 'S':
            case 'L':
                load_store(instruction, tag, set, setSize, lineSize, memory);
                break;
            case 'M':
                modify(instruction, tag, set, setSize, lineSize, memory);
                break;
            default:
                printf("wrong access type.\n");
                exit(EXIT_FAILURE);
                break;
            }
        }
    }
    //test(setSize, lineSize, blockSize, memory);
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
    return 0;
}