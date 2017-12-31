#include "proxy.h"

#include <iostream>

#include <sys/socket.h> // socket(), setsockopt()
#include <arpa/inet.h> // sockaddr_in, htons, inet_ntoa, ntohs


using namespace std;

HTTPProxy::HTTPProxy(int port){
    CreateServerSocket(port);

}


/* public methods */

void HTTPProxy::ProxyRequest(){
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int connectionfd = accept(mSocketDescriptor, (struct sockaddr *) &clientAddr, &clientAddrSize);
    
    const char *clientIPAddress = inet_ntoa(clientAddr.sin_addr);
    unsigned short clientPort = ntohs(clientAddr.sin_port);
    
    cout << "server got connection:" << inet_ntoa(clientAddr.sin_addr) << ntohs(clientAddr.sin_port) << endl;
    
    send(connectionfd, "Hello, world!\n", 13, 0);
    
    

}

/* private init methods */

void HTTPProxy::CreateServerSocket(int port){

    mSocketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    
    const int optval = 1;
    // allow other sockets to bind to this port, "Address already in use" error 
    setsockopt(mSocketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optval , sizeof(int)); 
    
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr)); // memset
    serverAddr.sin_family = AF_INET;
    // automatically be filled with current host's IP address
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serverAddr.sin_port = htons(port);
    struct sockaddr *sa = (struct sockaddr *) &serverAddr;
    
    ::bind(mSocketDescriptor, sa, sizeof(serverAddr)); // c libs instead of std::bind
    
    const size_t kMaxQueuedRequests = 128;
    listen(mSocketDescriptor, kMaxQueuedRequests);
    cout << "listening on port: " << port << endl;
    cout << serverAddr.sin_addr.s_addr << endl;

}