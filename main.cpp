/*

network programming:
http://www.bogotobogo.com/cplusplus/sockets_server_client.php
http://beej.us/guide/bgnet/html/multi/index.html
https://github.com/angrave/SystemProgramming/wiki/Networking,-Part-2:-Using-getaddrinfo
https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/netdb.h/getaddrinfo

style:
https://google.github.io/styleguide/cppguide.html
https://users.ece.cmu.edu/~eno/coding/CppCodingStandard.html
*/
#include <iostream>
#include <bits/stdc++.h>
#include "proxy.h"
#include "proxy_parse.h"

using namespace std;

int main(int argc, char *argv[]){
    // parse argv

    HTTPProxy httpproxy(3500);
    
    // cout << "listening on port: " << httpproxy.GetPort() << endl;
    while(true){
        httpproxy.ProxyRequest();
    }

    return 0;
}