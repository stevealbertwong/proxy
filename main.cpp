/*
Reference:
network programming:
http://www.bogotobogo.com/cplusplus/sockets_server_client.php
http://beej.us/guide/bgnet/html/multi/index.html
https://github.com/angrave/SystemProgramming/wiki/Networking,-Part-2:-Using-getaddrinfo
https://en.wikibooks.org/wiki/C_Programming/POSIX_Reference/netdb.h/getaddrinfo
http://www.cs.princeton.edu/courses/archive/spr15/cos461/assignments/1-http.html
https://quip.com/uuPUAKEPaEyu

style:
https://google.github.io/styleguide/cppguide.html
https://users.ece.cmu.edu/~eno/coding/CppCodingStandard.html
https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet
https://support.zendesk.com/hc/en-us/articles/203691016-Formatting-text-with-Markdown
*/
#include <iostream>
// https://stackoverflow.com/questions/25311011/how-does-include-bits-stdc-h-work-in-c
#include <bits/stdc++.h> // includes every standard library and stl include file, unnecesserary stuff and increases compilation time
#include <unistd.h> // fork(), close()
#include <sys/socket.h>
#include "proxy.h"
#include "proxy_parse.h"

using namespace std;

int main(int argc, char *argv[]){
    int mSocketDescriptor;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);

    HTTPProxy httpproxy(3500);
    
    // cout << "listening on port: " << httpproxy.GetPort() << endl;
    while(true){
        cout << "proxy request" << endl;
        int client_fd = accept(mSocketDescriptor, (struct sockaddr *) &clientAddr, &clientAddrSize);
        int pid = fork();
        
        if(pid == 0){
            httpproxy.ProxyRequest(client_fd, clientAddr);
            close(client_fd);
            _exit(0);
        }else{
            close(client_fd);
        }
        
    }

    return 0;
}