## g++ -g -Wall -pedantic -O0 -std=c++0x -x cc main.cc proxy.cc -x c proxy_parse.c -o http-proxy

CXX = g++
CXXFLAGS = -g -Wall 

SOURCES = \
	main.cpp \
	proxy.cpp 
HEADERS = $(SOURCES:.cpp=.h)
OBJECTS = $(SOURCES:.cpp=.o)
TARGETS = http-proxy


all: http-proxy

http-proxy: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o proxy_parse.o -c proxy_parse.c 
	$(CXX) $(CXXFLAGS) -o proxy.o -c proxy.cpp
	$(CXX) $(CXXFLAGS) -o main.o -c main.cpp
	$(CXX) $(CXXFLAGS)  -o proxy proxy_parse.o proxy.o main.o

clean:
	rm -f proxy *.o

# Makefile.dependencies:: $(SOURCES) $(HEADERS)
# 	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > Makefile.dependencies