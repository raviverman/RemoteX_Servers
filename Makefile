# Makefile for Separate Compilation Example

# *****************************************************
# Parameters to control Makefile operation

CXX = g++
CXXFLAGS = -pthread

# ****************************************************
# Entries to bring the executable up to date

main: main.o configuration.o executer.o
	$(CXX) $(CXXFLAGS) -o main main.o configuration.o executer.o

main.o: main.cpp def.h
	$(CXX)  -c main.cpp 

configuration.o: configuration.cpp def.h
	$(CXX) -c configuration.cpp 

executer.o: executer.cpp def.h
	$(CXX) $(CXXFLAGS) -c executer.cpp 

clean:
	rm ./*.o
