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
#include <unistd.h>
#include <bits/stdc++.h>
#include "proxy.h"
#include "proxy_parse.h"

using namespace std;

int main(int argc, char *argv[]){
    
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    HTTPProxy httpproxy(3500);
    int proxy_fd = getProxyfd();

    // cout << "listening on port: " << httpproxy.GetPort() << endl;
    while(true){

        int client_fd = accept(proxy_fd, (struct sockaddr *) &clientAddr, &clientAddrSize);
        
        int pid = fork();
        if(pid==0){
            // should i instead reference it so multiprocess share same global variable
            cout << "mp with a common client_fd" << endl;
            httpproxy.ProxyRequest(client_fd);
        }else{
            close(client_fd);
        }
    }

    return 0;
}