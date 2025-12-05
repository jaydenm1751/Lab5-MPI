# Compiler
CXX      = mpic++
CXXFLAGS = -O3 -ffast-math -march=native -Wall -Wextra -std=c++17 -Iinclude


# Executable name
TARGET   = lab5_mpi

# Source files
SRCS = src/main.cpp \
       src/bh.cpp   \
       src/io.cpp   \
       src/integrator.cpp \
       src/argparse.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp 
src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean builds
clean:
	rm -f $(OBJS) $(TARGET)
