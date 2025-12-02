//io.cpp
#include <fstream>
#include <iostream>
#include <iomanip>
#include "io.h"

std::vector<Body> load_bodies(const std::string& input_file){
    std::vector<Body> bodies;
    // int index
    // double x, y;
    // double mass;
    // double vx, vy;
    std::ifstream in(input_file);
    if (!in){
        std::cerr << "Cannot open input file: " << input_file << std::endl;
        return bodies;
    }
    int N;
    in >> N;
    bodies.reserve(N);
    for (int i = 0; i < N; i++){
        Body b;
        in >> b.index >> b.x >> b.y >> b.mass >> b.vx >> b.vy;

        bodies.push_back(b);
    }
    return bodies;
}


void write_output(const std::string& output_file, const std::vector<Body>& bodies) {
    int N = static_cast<int>(bodies.size());
    std::ofstream out(output_file);
    if (!out){
        std::cerr << "Bad output file: " << output_file << std::endl;
        return;
    }
    out << std::scientific << std::setprecision(6);
    out << N << "\n";
    for (const auto& b : bodies){
        out << b.index << " " << b.x << " " << b.y << " " << b.mass << " " << b.vx << " " << b.vy << "\n";
    }
}