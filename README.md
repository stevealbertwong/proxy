# proxy


// -x c++ -x c when compile both c++ and c sources tgt => -x c++ use c++ compiler 
clang++ -x c++ main.cpp proxy.cpp -x c proxy_parse.c -o http-proxy

g++ -g -Wall -Werror -o proxy.o -c proxy.cpp
g++ -g -Wall -Werror -o proxy_parse.o -c proxy_parse.c
g++ -g -Wall -Werror -o main.o -c main.cpp
g++ -g -Wall -Werror -o proxy proxy_parse.o proxy.o main.o
