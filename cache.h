#ifndef _http_cache_
#define _http_cache_

#include <string>

class HTTPCache {
    public:
        HTTPCache();
        bool shouldCache(char* request) const;
        
    private:
        std::string cacheDirectory;
        void ensureDirectoryExists(const std::string& directory, bool empty = false) const;                
};

#endif