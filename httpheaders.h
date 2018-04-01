#ifndef _HTTP_HEADERS_H_
#define _HTTP_HEADERS_H_

#include <unordered_map>
#include <string>
#include <stdio.h> // size_t
#include <iostream>


using namespace std;

class HttpHeaders{
public:
  HttpHeaders();
  void parseHeaders(const string& request);  
  const string& findHeaders(const string& key) const;
  bool containsHeaders(const string& key) const ;
  void addHeaders(const string& key, const int& value);
  void addHeaders(const string& key, const string& value);
  
  void removeHeaders(const string& key);
  
  // TODO:
  // size_t getTotalHeaders();
  // ModifyHeaders();  
  // engineerHeaders();
  // unordered_map<string, string> getHeaders();

  size_t getStart() const {return start;}
  size_t getEnd() const {return end;}

  // const std::string& getMethod() const { return method; }

// inherited subclass could access
protected:
  size_t start = 0;
  size_t end = 0;

// only httpheader class could access
private:
  unordered_map<string, string> headers;
};


#endif // _HTTP_HEADERS_H_