#ifndef _http_blacklist_
#define _http_blacklist_

#include <vector>
#include <string>
#include <regex>
using namespace std;

class HTTPBlacklist {
    public:
        HTTPBlacklist(const string& file_name);
        bool is_blacklisted(const string& website);
    private:
        std::vector<std::regex> blacklist_websites;
                
};

#endif