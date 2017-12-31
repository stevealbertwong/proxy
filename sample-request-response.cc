const char response[] =

    "HTTP/1.1 200 OK\n"
    "Date: Thu, 19 Feb 2009 12:27:04 GMT\n"
    "Server: Apache/2.2.3\n"
    "Last-Modified: Wed, 18 Jun 2003 16:05:58 GMT\n"
    "ETag: \"56d-9989200-1132c580\"\n"
    "Content-Type: text/html\n"
    "Content-Length: 15\n"
    "Accept-Ranges: bytes\n"
    "Connection: close\n"
    "\n"
    "sdfkjsdnbfkjbsf"    

    "HTTP/1.1 200 OK\r\n"
    "Server: nginx/1.2.1\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 13\r\n"
    "Connection: keep-alive\r\n"
    "\r\n"
    "<html><p> Hello, world!</p><html />";


const char request[] = 
    "GET http://www.google.com:80/index.html/ HTTP/1.0\r\n"
    "Content-Length:80\r\n"
    "If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT\r\n"
    "\r\n"

    "GET /uri.cgi HTTP/1.1\r\n"
    "User-Agent: Mozilla/5.0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    "Host: 127.0.0.1\r\n"
    "\r\n";