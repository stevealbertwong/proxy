#include "proxy.h"
#include "proxy_parse.h"
#include <bits/stdc++.h>

#include <iostream>

#include <sys/socket.h> // socket(), setsockopt(), recv(), getaddrinfo
#include <sys/types.h> // getaddrinfo
#include <arpa/inet.h> // sockaddr_in, htons, inet_ntoa, ntohs
#include <strings.h> // bzero (on linux), strlen, strcat, strcpy
#include <stdlib.h> //malloc
#include <unistd.h> // close()

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
    uint16_t clientPort = ntohs(clientAddr.sin_port);    
    cout << "server got connection from client:" << clientIPAddress << clientPort << endl;

    // forward client request to google
    int MAX_BUFFER_SIZE = 5000;
    char buf[MAX_BUFFER_SIZE];
    char *request_message = (char *) malloc(MAX_BUFFER_SIZE); 
    request_message[0] = '\0';
	int total_received_bits = 0;
    
    // receive n copy client request from stream to buf
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
    cout << "request_message" << request_message << endl;


    struct ParsedRequest *req;    // contains parsed request
    req = ParsedRequest_create();    
    ParsedRequest_parse(req, request_message, strlen(request_message));
    char* req_string = RequestToString(req);	
    cout << "req_string" << req_string << endl;

    // remote socket: connection to remote host e.g. google
    int remote_socket = CreateRemoteSocket(req->host, req->port);
    
    cout << "SendRequestRemote" << endl;
    SendRequestRemote(req_string, remote_socket, total_received_bits);
    cout << "ProxyBackClient" << endl;
    ProxyBackClient(client_fd, remote_socket);
    
    ParsedRequest_destroy(req);		
	close(client_fd);   
	close(remote_socket);
}

/* private init methods */

void HTTPProxy::ProxyBackClient(int client_fd, int remote_socket){
    int MAX_BUF_SIZE = 5000;
	int buff_length;
	char received_buf[MAX_BUF_SIZE];

    // receive from remote's response, send back to client
	while ((buff_length = recv(remote_socket, received_buf, MAX_BUF_SIZE, 0)) > 0) {
        int totalsent = 0;
        int senteach;
        while (totalsent < buff_length) {		
            if ((senteach = send(client_fd, (void *) (received_buf + totalsent), buff_length - totalsent, 0)) < 0) {
                fprintf (stderr," Error in sending to server ! \n");
                    exit (1);
            }
            totalsent += senteach;
		memset(received_buf,0,sizeof(received_buf));	
    	}      
    }
}

void HTTPProxy::SendRequestRemote(const char *req_string, int remote_socket, int buff_length){
	int totalsent = 0;
	int senteach;
	while (totalsent < buff_length) {
		if ((senteach = send(remote_socket, (void *) (req_string + totalsent), buff_length - totalsent, 0)) < 0) {
			fprintf (stderr," Error in sending to server ! \n");
				exit (1);
		}
		totalsent += senteach;
	}	
}

int HTTPProxy::CreateRemoteSocket(char* remote_addr, char* port){
    // given address and port, configure hints to get results about host name
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(remote_addr, port, &hints, &servinfo);

    // once get hostname info, creates remote socket n make a connection on socket
    int remote_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    connect(remote_socket, servinfo->ai_addr, servinfo->ai_addrlen);
    freeaddrinfo(servinfo);
    return remote_socket;
}

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