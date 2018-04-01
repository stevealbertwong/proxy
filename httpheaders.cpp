/*
class with header parsing methods and unordered_map of headers

*/
#include <sstream>
#include "httpheaders.h"

using namespace std;

// but if call in subclass here, add HttpHeader:: as scope if same name scope
HttpHeaders::HttpHeaders(){}

void HttpHeaders::parseHeaders(const string& request){    
  // start: starting position of request/response to find()
  while (true)
  {
      string key, value;
      start = getStart();
      end = getEnd();

      end = request.find(": ", start);
      if (end == string::npos)
          break;
      key = request.substr(start, end - start);
      start = end + 2;
      end = request.find("\r\n", start);
      if (end == string::npos)
          break;
      value = request.substr(start, end - start);
      start = end + 2;

      cout << "Key: " << key << " Value: " << value << endl;
      headers[key] = value;
  }
}

static const string kEmptyString;
const string& HttpHeaders::findHeaders(const string& key) const {
  auto found = headers.find(key);
  return found == headers.end() ? kEmptyString : found->second;  
}

bool HttpHeaders::containsHeaders(const string& key) const{
  return headers.find(key) != headers.end();
}

// if int, convert int to string then call twin brother function
void HttpHeaders::addHeaders(const string& key, const int& value){
  ostringstream oss;
  oss << value;
  addHeaders(key, oss.str());
}

void HttpHeaders::addHeaders(const string& key, const string& value){
  headers[key] = value;
}

void HttpHeaders::removeHeaders(const string& key){
  headers.erase(key);
}



