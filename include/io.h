// io.h
#pragma once

#include <string>
#include <vector>
#include "bh.h"   // needed for Body

// Loads bodies from the input file.
// Returns a vector<Body> with index, x, y, mass, vx, vy populated.

std::vector<Body> load_bodies(const std::string& input_file);

// Writes bodies to output file in scientific notation,
// matching the required output format.
void write_output(const std::string& output_file,
                  const std::vector<Body>& bodies);
