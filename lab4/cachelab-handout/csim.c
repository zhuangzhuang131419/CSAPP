#include "cachelab.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int verbose = 0; //verbose flag
int s;  //number of set index bits
int E;  //number of lines per set
int b;  //number of block bits
FILE* fp = NULL;
const char* usage = "Usage: %s [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n";

void parseArgument(int argc, char* argv[]);

int main(int argc, char* argv[])
{

        parseArgument(argc, argv);
        printf("s = %d, E = %d, b = %d\n", s, E, b);
        printSummary(1, 0, 0);
        return 0;
}

void parseArgument(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch(opt)
        {
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
}