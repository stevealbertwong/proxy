#include "proxy.h"
#include "proxy_parse.h"

#include <iostream>

#include <sys/socket.h> // socket(), setsockopt(), recv()
#include <sys/types.h> // getaddrinfo
#include <arpa/inet.h> // sockaddr_in, htons, inet_ntoa, ntohs
#include <strings.h> // bzero (on linux), strlen, strcat, strcpy
#include <stdlib.h> //malloc

#include <netdb.h> // getaddrinfo

using namespace std;

HTTPProxy::HTTPProxy(int port){
    CreateServerSocket(port);

}

/* public methods */

void HTTPProxy::ProxyRequest(){
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    // write incoming client's connection to sockaddr
    int client_fd = accept(mSocketDescriptor, (struct sockaddr *) &clientAddr, &clientAddrSize);


    
    const char *clientIPAddress = inet_ntoa(clientAddr.sin_addr);
    unsigned short clientPort = ntohs(clientAddr.sin_port);
    
    cout << "server got connection:" << clientIPAddress << clientPort << endl;

    // format response
    const char response[] =

        "HTTP/1.1 200 OK\r\n"
        "Server: nginx/1.2.1\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 13\r\n"
        "Connection: keep-alive\r\n"
        "\r\n"
        "<html><p> Hello, world!</p><html />";
    
    // send back to client
    // send(client_fd, response, strlen(response), 0);

    // forward client request to google
    int MAX_BUFFER_SIZE = 5000;
    char buf[MAX_BUFFER_SIZE];
    char *request_message = (char *) malloc(MAX_BUFFER_SIZE); 
    request_message[0] = '\0';
	int total_received_bits = 0;
    
    // copy client request from stream to buf
    while (strstr(request_message, "\r\n\r\n") == NULL) {
        int byte_recvd = recv(client_fd, buf, MAX_BUFFER_SIZE, 0);
        total_received_bits += byte_recvd;
        buf[byte_recvd] = '\0';
	  	if (total_received_bits > MAX_BUFFER_SIZE) {
			MAX_BUFFER_SIZE *= 2;
			request_message = (char *) realloc(request_message, MAX_BUFFER_SIZE);
        }
        strcat(request_message, buf);
    }

    struct ParsedRequest *req;    // contains parsed request
    req = ParsedRequest_create();
    ParsedRequest_parse(req, request_message, strlen(request_message));
    char*  browser_req  = RequestToString(req);	

    // create remote socket
    int remote_socket = 







        
        














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
}

char* HTTPProxy::RequestToString(struct ParsedRequest *req)
{

	/* Set headers */
	ParsedHeader_set(req, "Host", req -> host);
	ParsedHeader_set(req, "Connection", "close");

	int iHeadersLen = ParsedHeader_headersLen(req);

	char *headersBuf;

	headersBuf = (char*) malloc(iHeadersLen + 1);
	ParsedRequest_unparse_headers(req, headersBuf, iHeadersLen);
	headersBuf[iHeadersLen] = '\0';


	int request_size = strlen(req->method) + strlen(req->path) + strlen(req->version) + iHeadersLen + 4;
	
	char *serverReq;

	serverReq = (char *) malloc(request_size + 1);

	serverReq[0] = '\0';
	strcpy(serverReq, req->method);
	strcat(serverReq, " ");
	strcat(serverReq, req->path);
	strcat(serverReq, " ");
	strcat(serverReq, req->version);
	strcat(serverReq, "\r\n");
	strcat(serverReq, headersBuf);

	free(headersBuf);

	return serverReq;

}