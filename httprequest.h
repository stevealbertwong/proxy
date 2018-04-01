#ifndef _HTTP_REQUEST_
#define _HTTP_REQUEST_

#include "httpheaders.h"
#include <string>

class HttpRequest : public HttpHeaders{
public:
	HttpRequest();
	void parseRequest(const string& request);

private:
	string method;
	string uri;
	string version; // protocol

};

#endif