# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

all: test-base64.exe

test-base64.exe: test-base64.o base64.o
	$(CXX) $(CXXFLAGS) -o test-base64.exe test-base64.o base64.o

# The *.o targets can be written more simply

test-base64.o: test-base64.cpp base64-mask.hpp
	$(CXX) $(CXXFLAGS) -c test-base64.cpp

base64.o: base64.cpp base64-mask.hpp

clean:
	 del *.o *.exe