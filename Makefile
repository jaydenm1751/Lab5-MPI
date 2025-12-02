# Compiler
CXX = mpic++
CXXFLAGS = -O2 -Wall -Wextra -std=c++17

# Executable name
TARGET = lab5_seq

# Source files
SRCS = main.cpp \
       bh.cpp \
       io.cpp \
       integrator.cpp \
       argparse.cpp

# Header files (not compiled, but trigger rebuild)
HDRS = bh.h \
       io.h \
       integrator.h \
       argparse.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Link step
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile .cpp → .o
%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<

# Run the program (example)
run: $(TARGET)
	./$(TARGET) -i input.txt -o out.txt -s 10 -t 0.5 -d 0.01

# Clean builds
clean:
	rm -f $(OBJS) $(TARGET)
