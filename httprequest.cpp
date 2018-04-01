#include "httprequest.h"
#include <iostream>

HttpRequest::HttpRequest(){}

void HttpRequest::parseRequest(const string& request){
	start = getStart();
    end = getEnd();
  
    // parse request line: Get method, URI, and version info
    end = request.find(' ', start);
    if (end == string::npos)
        return ;
    method = request.substr(start, end - start);
    start = end + 1;
    end = request.find(' ', start);
    if (end == string::npos)
        return ;
    uri = request.substr(start, end - start);
    start = end + 1;
    end = request.find("\r\n", start);
    if (end == string::npos)
        return ;
    version = request.substr(start, end - start);
    start = end + 2;

    cout << "method: " << method << endl;
    cout << "uri: " << uri << endl;
    cout << "version: " << version << endl;

  	return HttpHeaders::parseHeaders(request);	
}