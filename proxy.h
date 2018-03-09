#ifndef _http_proxy_
#define _http_proxy_

#include "proxy_parse.h"

class HTTPProxy {
    public:
        HTTPProxy(int port);
        int WaitToAccept();
        void ProxyRequest(int client_fd);

    private:
        int mSocketDescriptor;
        void ProxyBackClient(int client_fd, int remote_socket);
        void SendRequestRemote(const char *req_string, int remote_socket, int buff_length);
        int CreateRemoteSocket(char* remote_addr, char* port);
        void CreateServerSocket(int port);
        char* RequestToString(struct ParsedRequest *req);        
};

#endif