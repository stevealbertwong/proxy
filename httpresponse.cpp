#include "httpresponse.h"

HttpResponse::HttpResponse(){}

void HttpResponse::parseResponse(const string& response){

    start = getStart();
    end = getEnd();
  
    // parse response line: Get method, URI, and version info
    end = response.find(' ', start);
    if (end == string::npos)
        return ;
    version = response.substr(start, end - start);
    start = end + 1;
    end = response.find(' ', start);
    if (end == string::npos)
        return ;
    status = response.substr(start, end - start);
    start = end + 1;
    end = response.find("\r\n", start);
    if (end == string::npos)
        return ;
    status_line = response.substr(start, end - start);
    start = end + 2;

    cout << "Http version: " << version << endl;
    cout << "status: " << status << endl;
    cout << "status_line: " << status_line << endl;

  return HttpHeaders::parseHeaders(response);
  // return ParsedHeader();
}

