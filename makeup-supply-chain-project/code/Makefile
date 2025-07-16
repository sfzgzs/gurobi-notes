# Compiler and flags
CXX = g++-9
CXXFLAGS = -O2 -std=c++11 -I. -I$(GUROBI_HOME)/include -std=c++11

# Gurobi paths
GUROBI_HOME = /opt/gurobi1202/linux64
LIBS = $(GUROBI_HOME)/lib/libgurobi_c++.a -L$(GUROBI_HOME)/lib -lgurobi120 -lpthread -lm

# Source files and output binary
SOURCES = main.cpp SKU.cpp utils.cpp
OUTPUT = main

.PHONY: all clean run

all:
	$(CXX) $(CXXFLAGS) -o $(OUTPUT) $(SOURCES) $(LIBS)

run: all
	LD_LIBRARY_PATH=$(GUROBI_HOME)/lib ./$(OUTPUT)

clean:
	rm -f $(OUTPUT)
