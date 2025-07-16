# Compiler and flags
CXX = g++-9
CXXFLAGS = -O2 -std=c++11

# Gurobi paths
GUROBI_HOME = /opt/gurobi1202/linux64
INCLUDE = -I$(GUROBI_HOME)/include
LIBS = $(GUROBI_HOME)/lib/libgurobi_c++.a -L$(GUROBI_HOME)/lib -lgurobi120 -lpthread -lm

# Source files and output binary
SOURCES = main.cpp SKU.cpp utils.cpp
OUTPUT = main

# Default target
all: $(OUTPUT)

# Build rule: compile and link
$(OUTPUT): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(OUTPUT) $(SOURCES) $(LIBS)

# Clean rule: remove built binary
.PHONY: clean
clean:
	rm -f $(OUTPUT)

# Run rule: run the program with correct library path
.PHONY: run
run: $(OUTPUT)
	LD_LIBRARY_PATH=$(GUROBI_HOME)/lib ./$(OUTPUT)