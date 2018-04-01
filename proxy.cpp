#include <bits/stdc++.h>

#include <iostream>

#include <sys/socket.h> // socket(), setsockopt(), recv(), getaddrinfo
#include <sys/types.h> // getaddrinfo
#include <arpa/inet.h> // sockaddr_in, htons, inet_ntoa, ntohs
#include <strings.h> // bzero (on linux), strlen, strcat, strcpy
#include <stdlib.h> //malloc
#include <unistd.h> // close(), getcwd()
#include <string>

#include <netdb.h> // getaddrinfo

#include "proxy.h"
#include "proxy_parse.h"
#include "blacklist.h"

using namespace std;

HTTPProxy::HTTPProxy(int port){
    CreateProxySocket(port);
    
}

/* public methods */
void HTTPProxy::ProxyRequest(int client_fd, struct sockaddr_in clientAddr, socklen_t clientAddrSize){                
    
    const char *clientIPAddress = inet_ntoa(clientAddr.sin_addr);
    uint16_t clientPort = ntohs(clientAddr.sin_port);    
    cout << "server got connection from client firefox browser:" << clientIPAddress << "port no.: " << clientPort << endl;


    // forward client request to google
    int MAX_BUFFER_SIZE = 5000;
    char buf[MAX_BUFFER_SIZE];
    char *request_message = (char *) malloc(MAX_BUFFER_SIZE); 
    
    // error checking
    if (request_message == NULL) {
		fprintf (stderr," Error in memory allocation ! \n");
		
	}
    cout << "passed error checking requst msg not null " << endl;

    request_message[0] = '\0';
	int total_received_bits = 0;
    
    // receive n copy client request from stream to buf
    while (strstr(request_message, "\r\n\r\n") == NULL) {
        int byte_recvd = recv(client_fd, buf, MAX_BUFFER_SIZE, 0);
        
        // error checking
        if (byte_recvd<0){
            fprintf(stderr,"error in reciving from client's request");
      
        } else if(byte_recvd ==0){
            fprintf(stderr,"zero byte received");
            break;
        }else{
            total_received_bits += byte_recvd;
            buf[byte_recvd] = '\0';
            if (total_received_bits > MAX_BUFFER_SIZE) {
                MAX_BUFFER_SIZE *= 2;
                request_message = (char *) realloc(request_message, MAX_BUFFER_SIZE);
            }

        }        
        strcat(request_message, buf);
    }
    cout << "request_message in char*: " << request_message << endl;
    

    // TODO => parse request_message into http-request class

    // char cCurrentPath[FILENAME_MAX];
    // getcwd(cCurrentPath, sizeof(cCurrentPath));
    // cCurrentPath[sizeof(cCurrentPath) - 1] = '\0';
    // string blacklistFile = cCurrentPath + "/blocked-domains.txt"
    
    HTTPBlacklist blacklist("blocked-domains.txt");
    cout << "blacklist initialized" << request_message << endl;
    
    if(blacklist.is_blacklisted(request_message)){
        cout << "blacklisted server: " << request_message << endl;
        exit(0);
    }
    
    struct ParsedRequest *req;    // contains parsed request
    req = ParsedRequest_create();    
    
    if(ParsedRequest_parse(req, request_message, strlen(request_message))<0){
        cout << "request message format not supported yet" << endl;
        ParsedRequest_destroy(req);
        // !!!! remember to close clientfd for failed request otherwise stuck at parse_request loop, client_fd is blocked
        close(client_fd); 
        
    } else {
        // default port set to 80
        if (req->port == NULL) {
            req->port = (char *) "80";
        }		 
        char* req_string = EngineerRequest(req);	
        cout << "client req_string to be sent to google : " << req_string << endl;

        cout << "client host n port: " << req->host << req->port << endl;
        // remote socket: connection to remote host e.g. google
        int remote_socket = CreateRemoteSocket(req->host, req->port);
        
        string reqt(req_string);
        if(httpcache.ensureEntryExists(reqt)){
            cout << "cached !! " << endl;
            string res = httpcache.retrieveCache(reqt);
            ProxyBackClient(client_fd, remote_socket, req_string);
        }
        else{
            cout << "SendRequestRemote: " << remote_socket << " total received bits" << total_received_bits << endl;
            SendRequestRemote(req_string, remote_socket, total_received_bits);
            cout << "ProxyBackClient" << endl;
            ProxyBackClient(client_fd, remote_socket, req_string);        
        }

        ParsedRequest_destroy(req);		
        close(client_fd);   
        close(remote_socket);
    }
}

/* private init methods */
//TODO: save response stream as string + parse + save string into directory
void HTTPProxy::ProxyBackClient(int client_fd, int remote_socket, const char* req_string){
    string request;
    string full_response;
	request.append(req_string);
    int MAX_BUF_SIZE = 5000;
	int buff_length;
	char received_buf[MAX_BUF_SIZE];

    // receive from remote's response, send back to client
	while ((buff_length = recv(remote_socket, received_buf, MAX_BUF_SIZE, 0)) > 0) {
        
        cout << "received from remote: "<< buff_length << endl;
        
        full_response.append(received_buf);
        string temp;
	    temp.append(received_buf);
        int totalsent = 0;
        int senteach;
        while (totalsent < buff_length) {		
            if ((senteach = send(client_fd, (void *) (received_buf + totalsent), buff_length - totalsent, 0)) < 0) {                
                fprintf (stderr," Error in sending to server ! \n");                
            }
            totalsent += senteach;
            cout << "sending back to client" << totalsent << endl;		    
    	}      
        memset(received_buf,0,sizeof(received_buf));
    }
    httpcache.saveCache(request, full_response);
    cout << "whether cache exist: " << httpcache.ensureEntryExists(request) << endl;
}

void HTTPProxy::SendRequestRemote(const char *req_string, int remote_socket, int buff_length){
	string temp;
	temp.append(req_string);
    int totalsent = 0;
    int senteach;
    cout << "SendRequestRemote : "<< totalsent << " , " << buff_length << endl;
	while (totalsent < buff_length) {
        cout << "about to send to remote" << endl;
		if ((senteach = send(remote_socket, (void *) (req_string + totalsent), buff_length - totalsent, 0)) < 0) {
            cout << "error sending ot remote" << endl;
        }        
        // senteach = send(remote_socket, (void *) (req_string + totalsent), buff_length - totalsent, 0);
        cout << "sent to remote" << senteach <<  endl;
		totalsent += senteach;
        cout << "total sent to remote: " << totalsent << endl;
	}	
}

int HTTPProxy::CreateRemoteSocket(char* remote_addr, char* port){
    // given address and port, configure hints to get results about host name
    struct addrinfo hints, *servinfo;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
    hints.ai_socktype = SOCK_STREAM;
    
    if (getaddrinfo(remote_addr, port, &hints, &servinfo) !=0){
        cout << " Error in server address format ! \n" << endl;
    }

    // once get hostname info, creates remote socket n make a connection on socket
    int remote_socket;
    if((remote_socket = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol))<0) {
        cout << " Error in creating socket to server ! \n" << endl;
    }
    
    if(connect(remote_socket, servinfo->ai_addr, servinfo->ai_addrlen) <0){
        cout << " Error in connecting to server ! \n" << endl;
    }
    
    freeaddrinfo(servinfo);
    return remote_socket;
}

void HTTPProxy::CreateProxySocket(int port){

    proxy_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    const int optval = 1;
    // allow other sockets to bind to this port, "Address already in use" error 
    setsockopt(proxy_fd, SOL_SOCKET, SO_REUSEADDR, &optval , sizeof(int)); 
    
    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr)); // memset
    serverAddr.sin_family = AF_INET;
    // automatically be filled with current host's IP address
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serverAddr.sin_port = htons(port);
    struct sockaddr *sa = (struct sockaddr *) &serverAddr;
    
    ::bind(proxy_fd, sa, sizeof(serverAddr)); // c libs instead of std::bind
    
    const size_t kMaxQueuedRequests = 128;
    listen(proxy_fd, kMaxQueuedRequests);
    cout << "listening on port: " << port << endl;    
}

// engineering request back
char* HTTPProxy::EngineerRequest(struct ParsedRequest *req)
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
    
    if(serverReq == NULL){
		fprintf (stderr," Error in memory allocation for serverrequest ! \n");
		
	}

	serverReq[0] = '\0'; // initialization of char string
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