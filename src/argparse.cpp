// argparse.cpp
#include <iostream>
#include <cstdlib>
#include <getopt.h>
#include "argparse.h"

void get_opts(int argc, char **argv, struct options_t *opts) {
    if (argc == 1) {
        std::cout << "Usage:" << std::endl;
        std::cout << "\t-i <file_path>" << std::endl;
        std::cout << "\t-o <file_path>" << std::endl;
        std::cout << "\t-s <iters>" << std::endl;
        std::cout << "\t-t <theta>" << std::endl;
        std::cout << "\t-d <dt>" << std::endl;
        std::cout << "\t-v [Optional flag]" << std::endl;
        exit(0);
    }

    opts->visualization = false;
    opts->dt = 0.005; //default according to spec

    struct option l_opts[] = {
        {"input_file", required_argument, NULL, 'i'},
        {"output_file", required_argument, NULL, 'o'},
        {"steps", required_argument, NULL, 's'},
        {"theta", required_argument, NULL, 't'},
        {"dt", required_argument, NULL, 'd'},
        {"visualization", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    int ind = 0;
    int c;
    while ((c = getopt_long(argc, argv, "i:o:s:t:d:v:", l_opts, &ind)) != -1) {
        switch (c) {
            case 0:
                // const char* name = l_opts[ind].name;
                // if (std::string(name) == "impl") {
                //     opts->impl = optarg; // "cpu" | "gpu" | "shmem" | "thrust"
                // }
                break;
            case 'i':
                opts->input_file = optarg;
                break;
            case 'o':
                opts->output_file = optarg;
                break;
            case 's':
                opts->steps = atoi(optarg);
                break;
            case 't':
                opts->theta = atof(optarg);
                break;
            case 'd':
                opts->dt = atof(optarg);
                break;
            case 'v':
                opts->visualization = true;;
                break;
            case ':':
                std::cerr << argv[0] << ": option -" << (char)optopt << " requires an argument." << std::endl;
                exit(1);
        }
    }
}