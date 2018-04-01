#ifndef _http_proxy_
#define _http_proxy_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "proxy_parse.h"
#include "blacklist.h"
#include "cache.h"
#include "httpresponse.h"

class HTTPProxy {
    public:
        HTTPProxy(int port);
        void ProxyRequest(int client_fd, struct sockaddr_in clientAddr, socklen_t clientAddrSize);
        // http://www.cplusplus.com/forum/beginner/6713/        
        int proxy_fd;        
        int getProxyfd(){return this->proxy_fd;}
        
    private:        
        void ProxyBackClient(int client_fd, int remote_socket, const char* req_string);
        void SendRequestRemote(const char *req_string, int remote_socket, int buff_length);
        int CreateRemoteSocket(char* remote_addr, char* port);
        void CreateProxySocket(int port);
        char* EngineerRequest(struct ParsedRequest *req);
        HttpCache httpcache;
        HttpResponse httpresponse;
};

#endif