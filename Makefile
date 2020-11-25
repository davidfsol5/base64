# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date
ifeq ($(OS), Windows_NT)
all: test-base64.exe

test-base64.exe: test-base64.o base64.o
	$(CXX) $(CXXFLAGS) -o test-base64.exe test-base64.o base64.o

test-base64.o: test-base64.cpp base64.hpp
	$(CXX) $(CXXFLAGS) -c test-base64.cpp

base64.o: base64.cpp base64.hpp

clean:
	 del *.obj *.exe *.o *.out

else
all: test-base64.out

test-base64.out: test-base64.o base64.o
	$(CXX) $(CXXFLAGS) -o test-base64.out test-base64.o base64.o

test-base64.o: test-base64.cpp base64.hpp
	$(CXX) $(CXXFLAGS) -c test-base64.cpp

base64.o: base64.cpp base64.hpp

clean:
	 rm --force *.obj *.exe *.o *.out

endif

