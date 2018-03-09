#ifndef _http_blacklist_
#define _http_blacklist_

#include <vector>
#include <string>
#include <regex>

class HTTPBlacklist {
    public:
        HTTPBlacklist(const char* file_name);
        bool is_blacklisted(const char* website);
    private:
        std::vector<std::regex> blacklist_websites;
                
};

#endif