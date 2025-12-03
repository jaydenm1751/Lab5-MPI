//main.cpp
#include <iostream>
#include <vector>
#include "bh.h"
#include "io.h"
#include "integrator.h"
#include "argparse.h"
#include <mpi.h>
using namespace std;

//getting different numbers now when i run it...


int main(int argc, char **argv) {

    //lets do mpi after i get the base working
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    options_t opts;
    get_opts(argc, argv, &opts);

    vector<Body> bodies;
    if (rank == 0) {
        bodies = load_bodies(opts.input_file);
        if (bodies.empty()){
            cerr << "Failed to load bodies\n";
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    int N = 0;
    if (rank == 0) {
        N = static_cast<int>(bodies.size());
    }
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank != 0){
        bodies.resize(N);
    }
    MPI_Bcast(bodies.data(), N * sizeof(Body), MPI_BYTE, 0, MPI_COMM_WORLD);

    vector<Body*> body_ptrs;
    body_ptrs.reserve(bodies.size());
    for (auto& b : bodies){
        body_ptrs.push_back(&b);
    }

    int local_start = (N * rank) / size;
    int local_end = (N * (rank + 1)) / size;

    //cout << "Hello from rank " << rank << " of " << size << endl;

    //next steps: divide up into different ranks and do work rank 0 can just do stuff in serial if needed

    if (rank == 0) {
        cout << "Lab5 MPI program starting with " << size << " processes, N = " << N << ".\n";
    }

    
    // options_t opts;
    // get_opts(argc, argv, &opts);
    // string output_file;
    // string input_file;
    // int steps = 0; //number of iterations
    // double theta = 0.0; //theta for determining if body is far away enough
    // double dt = 0.0; //timestep
    // bool visualization = false;

    // vector<Body> bodies = load_bodies(opts.input_file);
    // if (bodies.empty()){
    //     return -1;
    // }
    // vector<Body*> body_ptrs;
    // body_ptrs.reserve(bodies.size());
    // for (auto& b : bodies){
    //     body_ptrs.push_back(&b);
    // }
    // vector<double> Fx(bodies.size(), 0.0);
    // vector<double> Fy(bodies.size(), 0.0);

    for (int s = 0; s < opts.steps; s++){
        Node* root = BuildTree(body_ptrs);
        ComputeMassDistribution(root);
        vector<double> Fx_local(N, 0.0);
        vector<double> Fy_local(N, 0.0);

        for(int i = local_start; i < local_end; i++){
            Body* b = body_ptrs[i];
            if (b->mass < 0.0) continue;
            ComputeForceOnBody(b, root, opts.theta, Fx_local[i], Fy_local[i]);
        }

        vector<double> Fx_total, Fy_total;
        if (rank == 0){
            Fx_total.resize(N);
            Fy_total.resize(N);
        }

        MPI_Reduce(Fx_local.data(),
                   rank == 0 ? Fx_total.data() : nullptr,
                   N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(Fy_local.data(),
                   rank == 0 ? Fy_total.data() : nullptr,
                   N, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        
        if (rank == 0) {
            for (int i = 0; i < N; i++){
                Body* b = body_ptrs[i];
                if (b->mass < 0.0) continue;
                UpdateBody(b, Fx_total[i], Fy_total[i], opts.dt);
            }
        }        
        FreeTree(root);
        MPI_Bcast(bodies.data(), N * sizeof(Body), MPI_BYTE, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        write_output(opts.output_file, bodies);
    }
    //write_output(opts.output_file, bodies);

    MPI_Finalize();

    return 0;
}
