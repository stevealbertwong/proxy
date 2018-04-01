#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include <iostream>
#include <string>
#include "httpheaders.h"

// inheritance allows you to call httpresponse.ParseHeader() in main
// but if call in subclass here, add HttpHeader:: as scope if same name scope
// optional if no name clash
class HttpResponse : public HttpHeaders{
public:
	HttpResponse();
	void parseResponse(const string& response);
	
	// TODO: 
	// engineerResponse();
	// getVersion();
	// getStatus();
	// getStatusLine();

private:	
	string version;
    string status;
    string status_line;

};


#endif