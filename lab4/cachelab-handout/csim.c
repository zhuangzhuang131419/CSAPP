#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

typedef unsigned long int uint64_t;

typedef struct {
    int valid;
    int lru;
    uint64_t tag;
}CacheLine;

typedef CacheLine* CacheSet;
typedef CacheSet* Cache;

const char* usage = "Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n";

void parseArgument(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	int opt;
	int verbose = 0; //verbose flag
	int s;  //number of set index bits
	int E;  //number of lines per set
	int b;  //number of block bits
    	FILE* fp = NULL;
	while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        	switch(opt) {
            		case 'h':
                		fprintf(stdout, usage, argv[0]);
                		exit(1);
            		case 'v':
                		verbose = 1;
                		break;
            		case 's':
                		s = atoi(optarg);
                		break;
            		case 'E':
                		E = atoi(optarg);
                		break;
            		case 'b':
                		b = atoi(optarg);
                		break;
            		case 't':
                		fp = fopen(optarg, "r");
                		break;
            		default:
                		fprintf(stdout, usage, argv[0]);
                		exit(1);
        	}
    	}


	printf("v= %d, s = %d, E = %d, b = %d\n", verbose, s, E, b);	
    	int hits = 0;
	int misses = 0;
	int evictions = 0;

	int S = pow(2, s);
	Cache cache = (Cache)malloc(sizeof(CacheSet) * S);
	if (cache == NULL) return -1;
	for (int i = 0; i < S; i++) {
        	cache[i] = (CacheSet)calloc(E, sizeof(CacheLine));
        	if (cache[i] == NULL) return -1;
    	}

	char buf[20];
	char operation;
    	uint64_t address;
    	int size;
	while (fgets(buf, sizeof(buf), fp) != NULL) {

		if (buf[0] == 'I') {
			continue;
		}
		printf("line = %s", buf);
		sscanf(buf, " %c %lx,%d", &operation, &address, &size);
		printf("operation = %c, address = %lx, size = %d\n", operation, address, size);
		if (operation == 'M') {
			hits++;
		}

		uint64_t tag = address >> (s + b);
		unsigned int setIndex = address >> b & ((1 << s) - 1);

		CacheSet cacheset = cache[setIndex];


		int evict = -1;
		int empty = -1;
		for (int i = 0; i < E; i++) {
			if (!cacheset[i].valid) {
				empty = i;
				continue;	
			}

			if (tag == cacheset[i].tag) {
				hits++;
				break;
			} else {
				evict = rand() % E;
			}

		}

		printf("evict = %d\n", evict);

		misses++;

		if (empty == -1) {
			cacheset[empty].valid = 1;
        		cacheset[empty].tag = tag;
        		cacheset[empty].lru = 1;
		} else {
			cacheset[evict].valid = 1;
                        cacheset[evict].tag = tag;
        		evictions++;
		}

		switch (operation) {
			case 'M':
				break;
			case 'L':
				break;
			case 'S':
				break;
		}
		if (verbose) {
		}
		printSummary(hits, misses, evictions);

	}

	for (int i = 0; i < S; i++) {
		free(cache[i]);
	}
    	// free(cache);
    	fclose(fp);
	printSummary(hits, misses, evictions);
    	return 0;
}
