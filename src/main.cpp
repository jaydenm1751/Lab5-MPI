//main.cpp
#include <iostream>
#include <vector>
#include "bh.h"
#include "io.h"
#include "integrator.h"
#include "argparse.h"
#include <mpi.h>
using namespace std;



int main(int argc, char **argv) {

    //lets do mpi after i get the base working
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    cout << "Hello from rank " << rank << " of " << size << endl;

    if (rank == 0) {
        cout << "Lab5 MPI program starting with " << size << " processes.\n";

    
        options_t opts;
        get_opts(argc, argv, &opts);
        // string output_file;
        // string input_file;
        // int steps = 0; //number of iterations
        // double theta = 0.0; //theta for determining if body is far away enough
        // double dt = 0.0; //timestep
        // bool visualization = false;

        vector<Body> bodies = load_bodies(opts.input_file);
        if (bodies.empty()){
            return -1;
        }
        vector<Body*> body_ptrs;
        body_ptrs.reserve(bodies.size());
        for (auto& b : bodies){
            body_ptrs.push_back(&b);
        }
        // vector<double> Fx(bodies.size(), 0.0);
        // vector<double> Fy(bodies.size(), 0.0);

        for (int s = 0; s < opts.steps; s++){
            Node* root = BuildTree(body_ptrs);
            ComputeMassDistribution(root);
            vector<double> Fx(bodies.size(), 0.0);
            vector<double> Fy(bodies.size(), 0.0);

            for(size_t i = 0; i < body_ptrs.size(); i++){
                Body* b = body_ptrs[i];
                if (b->mass < 0.0) continue;
                ComputeForceOnBody(b, root, opts.theta, Fx[i], Fy[i]);
            }

            for (size_t i = 0; i < body_ptrs.size(); i++){
                Body* b = body_ptrs[i];
                if (b->mass < 0.0) continue;
                UpdateBody(b, Fx[i], Fy[i], opts.dt);
            }
            
            FreeTree(root);
        }

        write_output(opts.output_file, bodies);
    }

    MPI_Finalize();

// ============================================================================================ //

    // //lets do mpi after i get the base working
    // MPI_Init(&argc, &argv);

    // int rank, size;
    // MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // MPI_Comm_size(MPI_COMM_WORLD, &size);

    // if (rank == 0) {
    //     cout << "Lab 5 MPI program starting with " << size << " processes.\n";
    // }

    // cout << "Hello from rank " << rank << " of " << size << endl;

    // MPI_Finalize();


    return 0;
}
