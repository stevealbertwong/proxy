## g++ -g -Wall -pedantic -O0 -std=c++0x main.cc proxy.cc -o http-proxy

CXX = g++
CXXFLAGS = -g -Wall -pedantic -O0 -std=c++0x
LDFLAGS = -lsocket++

SOURCES = \
	main.cc \
	proxy.cc 
HEADERS = $(SOURCES:.cc=.h)
OBJECTS = $(SOURCES:.cc=.o)
TARGETS = http-proxy

http-proxy: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

Makefile.dependencies:: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -MM $(SOURCES) > Makefile.dependencies