//argparse.h
#ifndef _ARGPARSE_H
#define _ARGPARSE_H

#include <getopt.h>
#include <stdlib.h>
#include <iostream>

struct options_t {
    const char *input_file; //-i input filename
    const char *output_file; //-o output filename
    int steps; // -s number of iter
    double theta; // -t threshold for distance
    double dt; // -d timestep
    bool visualization; //-v optional default false
};

void get_opts(int argc, char **argv, struct options_t *opts);
#endif