## g++ -g -Wall -pedantic -O0 -std=c++0x -x cc main.cc proxy.cc -x c proxy_parse.c -o http-proxy

# compiler + assembler + linker
CXX = g++
CXXFLAGS = -g -Wall 

SOURCES = \
	main.cpp \
	proxy.cpp \
	proxy_parse.c 
HEADERS = $(SOURCES:.cpp=.h)
OBJECTS = $(SOURCES:.cpp=.o)
TARGETS = http-proxy


# "all" is name of the default target, running "make" without params would use it
all: http-proxy

# which object files are linked in executable
# executable: file1.o file2.o
http-proxy: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o proxy_parse.o -c proxy_parse.c 
	$(CXX) $(CXXFLAGS) -o proxy.o -c proxy.cpp
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp
	$(CXX) $(CXXFLAGS) -o http-proxy proxy_parse.o proxy.o main.o

clean:
	rm -f http-proxy *.o

# Makefile.dependencies:: $(SOURCES) $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > Makefile.dependencies