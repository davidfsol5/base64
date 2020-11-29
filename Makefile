# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date
ifeq ($(OS), Windows_NT)
all: decode-file.exe

decode-file.exe: decode-file.o base64.o
	$(CXX) $(CXXFLAGS) -o decode-file.exe decode-file.o base64.o

decode-file.o: decode-file.cpp base64.hpp
	$(CXX) $(CXXFLAGS) -c decode-file.cpp

base64.o: base64.cpp base64.hpp

clean:
	 del *.obj *.exe *.o *.out

else
all: decode-file.out

decode-file.out: decode-file.o base64.o
	$(CXX) $(CXXFLAGS) -o decode-file.out decode-file.o base64.o

decode-file.o: decode-file.cpp base64.hpp
	$(CXX) $(CXXFLAGS) -c decode-file.cpp

base64.o: base64.cpp base64.hpp

clean:
	 rm --force *.obj *.exe *.o *.out

endif

