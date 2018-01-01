#ifndef _http_proxy_
#define _http_proxy_

class HTTPProxy {
    public:
        HTTPProxy(int port);
        void ProxyRequest();

    private:
        int mSocketDescriptor;
        void CreateServerSocket(int port);
        char* RequestToString(struct ParsedRequest *req);
        
};

#endif